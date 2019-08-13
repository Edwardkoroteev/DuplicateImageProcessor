//ADDING THREADS TO SPEED UP THE COMPUTATION
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/img_hash.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

//void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, int argc, char** argv);
bool compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, string imgOnePath, string imgTwoPath);
void readImg(int argc, char** argv);
void getPathName(vector<string> &imgPathsJpg, vector<string> &imgPathsPng, string path);
string filterFileName(string fileName);
void comparingImages(vector<string> smaller, vector<string> larger, string pathTo);
void compareDiffFormatImg(vector<string> imgPathsPng, vector<string> imgPathsJpg, string pathTo);
bool compareTwoImages(string imageOnePath, string imageTwoPath);
bool ifImageSameDisplay(string imgOnePath, string imgTwoPath, double similarityUnit);
bool isDir(string path);
void printing(vector<string> list);
void comparingSameFormatImage(vector<string> &imgPaths, string pathTo);
int fileSize(string path);
void moveFile(string from, string to);
bool removeFile(string path);
string filenameFromPath(string path);

int main(int argc, char** argv)
{
  vector<Mat> images;
  vector<string> imgPathsJpg;
  vector<string> imgPathsPng;
  vector<string> listNonDuplicates;
  //string path = "/Users/kedward/Desktop/images/";
  //string pathTo = "/Users/kedward/Desktop/test/";
  string path;
  string pathTo;

  cout << "Enter path of images: ";
  cin >> path;
  cout << "Enter path to move duplicate images to: ";
  cin >> pathTo;

  getPathName(imgPathsJpg, imgPathsPng, path);
  cout << "images (JPEG) size: " << imgPathsJpg.size() << endl;
  cout << "images (PNG) size: " << imgPathsPng.size() << endl;
  cout << "Total: " << imgPathsPng.size() + imgPathsJpg.size() << endl;

  if(imgPathsJpg.size() > 1 || imgPathsPng.size() > 1)
  {
    comparingSameFormatImage(imgPathsPng, pathTo);
    comparingSameFormatImage(imgPathsJpg, pathTo);
    compareDiffFormatImg(imgPathsPng, imgPathsJpg, pathTo);
  }
  else if(imgPathsJpg.size() == 1 || imgPathsPng.size() == 1)
  {
    cout << "Need at least two images to compare" << endl;
  }
  else
  {
    cout << "No PNG or JPEG images found in path" << endl;
  }

  return 0;
}

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
void printing(vector<string> list)
{
  for(int i = 0; i < list.size(); i++)
  {
    cout << i << " " << list[i] << endl;
  }
}

void moveFile(string from, string to)
{
  string pathTo = to + filenameFromPath(from);

  if(rename(from.c_str(), pathTo.c_str()) != 0)
  {
    cout << "Cannot delete: " << from << endl;
  }
}

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

void getPathName(vector<string> &imgPathsJpg, vector<string> &imgPathsPng, string path)
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
    cout << "Unable to access path" << endl;
    return;
  }
}

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

void compareDiffFormatImg(vector<string> imgPathsPng, vector<string> imgPathsJpg, string pathTo)
{
  if(imgPathsPng.size() > imgPathsJpg.size())
  {
    comparingImages(imgPathsJpg, imgPathsPng, pathTo);
  }
  else
  {
    comparingImages(imgPathsPng, imgPathsJpg, pathTo);
  }
}

void comparingImages(vector<string> smaller, vector<string> larger, string pathTo)
{
  cout << "comparing" << endl;
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

bool compareTwoImages(string imageOnePath, string imageTwoPath)
{
  using namespace cv::img_hash;

  //disable opencl acceleration may(or may not) boost up speed of img_hash
  ocl::setUseOpenCL(false);

  //cout << "ColorMomentHash: ";
  return compute(ColorMomentHash::create(), imageOnePath, imageTwoPath);
}

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

void readImg(int argc, char** argv)
{
  Mat image;
  image = imread(argv[1], 1);

  if(!image.data)
  {
    cout << "No Image data" << endl;
    return;
  }

  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", image);

}

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
  //resize(similarImg, similarImg, {10, 10});
  //resize(input, input, {10, 10});
  algo->compute(input, hashInput);
  algo->compute(similarImg, hashSimilar);

  double similarityUnit = algo->compare(hashInput, hashSimilar);

  if(similarityUnit < 0.1 && similarityUnit >= 0)
  {
    return true;
    /*
       imshow(imgOnePath, input);
       imshow(imgTwoPath, similarImg);
       waitKey(0);
       destroyAllWindows();
       waitKey(1);

       if(ifImageSameDisplay(imgOnePath, imgTwoPath, similarityUnit) == true)
       return;
     */
  }
  return false;
}

bool ifImageSameDisplay(string imgOnePath, string imgTwoPath, double similarityUnit)
{
  string userInput;
  cout << "========Similar Images========" << endl;
  cout << "Similarity: " << similarityUnit << endl;
  cout << imgOnePath << endl;
  cout << imgTwoPath << endl;
  cout << "Are these images the same? ";
  cin >> userInput;
  return true;
}

int fileSize(string path)
{
  ifstream file(path.c_str(), ios::in);
  file.seekg(0, ios::end);
  auto fileSize = file.tellg();
  return fileSize;
}
