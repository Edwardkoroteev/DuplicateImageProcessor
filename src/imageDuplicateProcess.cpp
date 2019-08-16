/* 
   Copyright (c) 2019 Edwardkoroteev
   This program is released under the MIT License
   Please see the file 'LICENSE' in this distribution for
   license terms.
 */

#include "imageDuplicateProcess.h"

//=====================================================
//Function Name:  run()
//Input:          none
//return:         void
//Description:    runs the entire image duplication
//                program from the beginning.
//=====================================================
void run()
{
  vector<Mat> images;
  vector<string> imgPathsJpg;
  vector<string> imgPathsPng;
  vector<string> listNonDuplicates;
  string path;
  string pathTo;

  cout << "**Note: Directory paths must end with '/'**" << endl;


  if(confirmPath("Enter path of images", path) == false)
  {
    return;
  }

  if(confirmPath("Enter path to move dupliacte images to", pathTo) == false)
  {
    return;
  }

  if(getPathName(imgPathsJpg, imgPathsPng, path) == false)
  {
    return;
  }

  computeImages(imgPathsJpg, imgPathsPng, pathTo);
}

//=====================================================
//Function Name:  computeImages()
//Input:          vector<string>imgPathsJpg
//                vector<string>imgPathsPng
//                string pathTo
//return:         void
//Description:    Receieves the vector lists paths of
//                jpg and png images and determines 
//                if the image comparison can be ran
//                depending on amount of images.
//                Image comparison is processed
//                if able to.
//=====================================================
void computeImages(vector<string>imgPathsJpg, vector<string>imgPathsPng, string pathTo)
{
 
  cout << "images (JPEG) size: " << imgPathsJpg.size() << endl;
  cout << "images (PNG) size: " << imgPathsPng.size() << endl;
  cout << "Total: " << imgPathsPng.size() + imgPathsJpg.size() << endl;

  if(imgPathsJpg.size() > 1 || imgPathsPng.size() > 1)
  {
    cout << "Comparing PNG..." << endl;
    comparingSameFormatImage(imgPathsPng, pathTo);
    cout << "Done..." << endl << endl;
    cout << "Comparing JPEG..." << endl;
    comparingSameFormatImage(imgPathsJpg, pathTo);
    cout << "Done..." << endl << endl;
    cout << "Comparing PNG and JPEG..." << endl;
    compareDiffFormatImg(imgPathsPng, imgPathsJpg, pathTo);
    cout << "Done..." << endl;
  }
  else if(imgPathsJpg.size() == 1 || imgPathsPng.size() == 1)
  {
    cout << "Need at least two images to compare" << endl;
  }
  else
  {
    cout << "No PNG or JPEG images found in path" << endl;
  }
}

//=====================================================
//Function Name:  confirmPath()
//Input:          string message
//                string &path
//return:         bool
//Description:    Displays the message variable to the
//                user and will expect the user to
//                enter input. Will then confirm
//                if user entered a valid directory
//                path.
//=====================================================
bool confirmPath(string message, string &path)
{
  string userInput;
  do
  {
    cout << "Enter 'q' to quit" << endl;
    cout << message << ": ";
    cin >> userInput;
    
    if(userInput == "q")
    {
      return false;
    }
  }
  while(isDir(userInput) == false);
  
  path = userInput;
  return true;
}

//=====================================================
//Function Name:  filenameFromPath()
//Input:          string path
//return:         string
//Description:    Receives a pathname with filename 
//                and returns just the name of the
//                file.
//=====================================================
string filenameFromPath(string path)
{
  string delimiter = "/";
  size_t pos = 0;
  string token;

  while((pos = path.find(delimiter)) != string::npos)
  {
    token = path.substr(0, pos);
    path.erase(0, pos + delimiter.length());
  }

  return path;
}

//=====================================================
//Function Name:  moveFile()
//Input:          string from
//                string to
//return          void
//Description:    Receives a path of a file and
//                moves it to the path passed in.
//=====================================================
bool moveFile(string from, string to)
{
  string pathTo = to + filenameFromPath(from);

  if(rename(from.c_str(), pathTo.c_str()) != 0)
  {
    cout << "Cannot move: " << from << endl;
    return false;
  }
  return true;
}

//=====================================================
//Function Name:  isDir()
//Input:         string path
//return:         bool
//Description:    Checks to see if the path is a
//                directory and returns true if so.
//=====================================================
bool isDir(string path)
{
  if(path.at(path.length()-1) == '.')
  {
    return false;
  }

  struct stat pathStat;
  stat(path.c_str(), &pathStat);
  if(S_ISDIR(pathStat.st_mode) == 1)
    return true;

  return false;
}

//=====================================================
//Function Name:  getPathName()
//Input:          vector<string> &imgPathsJpg
//                vector<string> &imgPathsPng
//                string path
//Return:         bool
//Description:    Starts the search from the given
//                path. Determines if files in path
//                are JPEG or PNG images and adds path
//                to appropriate list.
//=====================================================
bool getPathName(vector<string> &imgPathsJpg, vector<string> &imgPathsPng, string path)
{
  DIR *dir;
  struct dirent *dirp;
  vector<string>paths;
  string fileType;

  if ((dir = opendir(path.c_str())) != NULL)
  {
    while((dirp = readdir(dir)) != NULL)
    {
      fileType = filterFileName(dirp->d_name);
      if(isDir(path + dirp->d_name) == true)
      {
        getPathName(imgPathsJpg, imgPathsPng, path + dirp->d_name + "/");
      }
      else if(fileType == "jpg")
      {
        if(find(imgPathsJpg.begin(), imgPathsJpg.end(), path + dirp->d_name) == imgPathsJpg.end())
        {
          imgPathsJpg.push_back(path + dirp->d_name);
        }
      }
      else if(fileType == "png")
      {
        if(find(imgPathsPng.begin(), imgPathsPng.end(), path + dirp->d_name) == imgPathsPng.end())
        {
          imgPathsPng.push_back(path + dirp->d_name);
        }
      }
    }
    closedir(dir);
  }
  else
  {
    return false;
  }

  return true;
}

//=====================================================
//Function Name:  comparingSameFormatImage()
//Input:          vector<string> &imgPaths
//                string pathTo
//Return:         void
//Description:    Loops through the path vector passed
//                in to check if file size is the same.
//                If so, file is moved to the pathTo.
//=====================================================
void comparingSameFormatImage(vector<string> &imgPaths, string pathTo)
{
  Mat img1;
  Mat img2;
  int size1;
  int size2;

  for(int i = 0; i < imgPaths.size(); i++)
  {
    size1 = fileSize(imgPaths[i]);

    for(int j = i + 1; j < imgPaths.size(); j++)
    {
      size2 = fileSize(imgPaths[j]);

      if(size1 == size2)
      {
        //move file
        cout << "    Found: " << imgPaths[j] << endl;
        moveFile(imgPaths[j], pathTo);
        imgPaths.erase(imgPaths.begin() + j);
        j = j - 1;
      }
    }
  }
}

//=====================================================
//Function Name:  compareDiffFormatImg()
//Input:          vector<string> imgPathsPng
//                vector<string> imgPathsJpg
//                string pathTo
//Return:         void
//Description:    Determines if imgPathsPng or
//                imgPathsJpg is larger, and calls
//                comparingImages() function
//                appropriately.
//=====================================================
void compareDiffFormatImg(vector<string> imgPathsPng, vector<string> imgPathsJpg, string pathTo)
{
  if(imgPathsPng.size() > imgPathsJpg.size())
  {
    comparingImages(imgPathsPng, imgPathsJpg, pathTo);
  }
  else
  {
    comparingImages(imgPathsJpg, imgPathsPng, pathTo);
  }
}

//=====================================================
//Function Name:  comparingImages()
//Input:          vector<string> smaller
//                vector<string> larger
//                string pathTo
//Return:         void
//Description:    Double loop through the vector with
//                smaller vector on the outer loop.
//                Calls compareTwoImages() function
//                to determine if images are a match.
//                If so, image is moved to pathTo and
//                path is deleted from larger vector.
//=====================================================
void comparingImages(vector<string> smaller, vector<string> larger, string pathTo)
{
  for(int i = 0; i < smaller.size(); i++)
  {
    cout << i+1 << ": " << smaller[i] << endl;
    for(int j = 0; j < larger.size(); j++)
    {
      if(compareTwoImages(smaller[i], larger[j]) == true)
      {
        cout << "    Found: " << larger[j] << endl;
        moveFile(larger[j], pathTo);
        larger.erase(larger.begin() + j);
      }
    }
  }
}

//=====================================================
//Function Name:  compareTwoImages()
//Input:          string imageOnePath
//                string imageTwoPath
//Return:         bool
//Description:    Initializes ColorMomentHash and 
//                calls the compute() function and
//                passes path of imageOnePath and
//                imageTwoPath.
//=====================================================
bool compareTwoImages(string imageOnePath, string imageTwoPath)
{
  using namespace cv::img_hash;
  ocl::setUseOpenCL(false);

  return compute(ColorMomentHash::create(), imageOnePath, imageTwoPath);
}

//=====================================================
//Function Name:  filterFileName()
//Input:          string filename
//Return:         string
//Description:    Receives a path to a file and
//                determines if file is jpeg of png.
//                returns file extension of file.
//=====================================================
string filterFileName(string fileName)
{
  string::size_type n;

  string extensionJpg[4] = {".jpg", ".JPG", ".jpeg", ".JPEG"};
  for(int i = 0; i < 4; i++)
  {
    n = fileName.find(extensionJpg[i]);
    if(n != string::npos)
    {
      return "jpg";
    }
  }

  string extensionPng[2] = {".png", ".PNG"};
  for(int i = 0; i < 2; i++)
  {
    n = fileName.find(extensionPng[i]);

    if(n != string::npos)
    {
      return "png";
    }
  }

  return "none";
}

//=====================================================
//Function Name:  compute()
//Input:          cv::Ptr<cv::img_hash::ImgHashBase> algo
//                string imgOnePath
//                string imgTwoPath
//Return:         bool
//Description:    Reads images from imgOnePath and
//                imgTwoPath and uses the
//                algorithm hash to compute the
//                differences between the images
//                passed in. Returns true if images
//                are the same.
//=====================================================
bool compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, string imgOnePath, string imgTwoPath)
{

  Mat input = imread(imgOnePath.c_str(), 1);
  if(!input.data)
  {
    cout << "No Image Data: " << imgOnePath << endl;
    return false;
  }

  Mat similarImg = imread(imgTwoPath.c_str(), 1);
  if(!similarImg.data)
  {
    cout << "No Image Data: " << imgTwoPath << endl;
    return false;
  }

  Mat hashInput;
  Mat hashSimilar;
  algo->compute(input, hashInput);
  algo->compute(similarImg, hashSimilar);

  double similarityUnit = algo->compare(hashInput, hashSimilar);

  if(similarityUnit < 0.1 && similarityUnit >= 0)
  {
    return true;
  }
  return false;
}

//=====================================================
//Function Name:  fileSize()
//Input:          string path
//Return:         int
//Description:    Determines the size of the file
//                from the path passed in and returns
//                the amount of bytes the file is.
//=====================================================
int fileSize(string path)
{
  ifstream file(path.c_str(), ios::in);
  file.seekg(0, ios::end);
  auto fileSize = file.tellg();
  return fileSize;
}
