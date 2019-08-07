#include <iostream>
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

using namespace cv;
using namespace std;

//void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, int argc, char** argv);
void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, string imgOnePath, string imgTwoPath);
void readImg(int argc, char** argv);
void getPathName(vector<string> &imgPaths, string path);
bool filterFileName(string fileName);
void uploadImages(vector<Mat> &images, vector<string> imgPaths);
void comparingImages(vector<string> imgPaths);
void compareTwoImages(string imageOnePath, string imageTwoPath);
bool ifImageSameDisplay(string imgOnePath, string imgTwoPath, double similarityUnit);
bool isDir(string path);
void printing(vector<string> list);

int main(int argc, char** argv)
{
  vector<Mat> images;
  vector<string> imgPaths;
  string path = "/Users/kedward/Desktop/t/";

  getPathName(imgPaths, path);
  cout << "images size: " << imgPaths.size() << endl;

  printing(imgPaths);

  return 0;
}

void printing(vector<string> list)
{
  for(int i = 0; i < list.size(); i++)
  {
    cout << list[i] << endl;
  }
}

bool isDir(string path)
{
  if(path.at(path.length()-1) == '.')
  {
    cout << path << endl;
    return false;
  }

  struct stat pathStat;
  stat(path.c_str(), &pathStat);
  if(S_ISDIR(pathStat.st_mode) == 1)
    return true;

  return false;
}

void getPathName(vector<string> &imgPaths, string path)
{
  DIR *dir;
  struct dirent *dirp;
  vector<string>paths;

  if ((dir = opendir(path.c_str())) != NULL)
  {
    while((dirp = readdir(dir)) != NULL)
    {
      if(isDir(path + dirp->d_name) == true)
      {
        getPathName(imgPaths, path + dirp->d_name + "/");
      }
      else if(filterFileName(dirp->d_name) == true)
      {
        imgPaths.push_back(path+dirp->d_name);
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

void uploadImages(vector<Mat> &images, vector<string> imgPaths)
{
  cout << "Uploading Images..." << endl;
  Mat input;
  for(int i = 0; i < imgPaths.size(); i++)
  {
    input = imread(imgPaths[i].c_str(), 1);

    if(input.data)
    {
      cout << i+1 << ": " << imgPaths[i] << endl;
      images.push_back(input);
    }
    else
    {
      cout << "Unable to upload: " << imgPaths[i] << endl;
    }
  }
  cout << "Uploading Complete..." << endl;
}

void comparingImages(vector<string> imgPaths)
{
  cout << "comparing" << endl;
  for(int i = 0; i < imgPaths.size(); i++)
  {
    cout << i << ": " << imgPaths[i] << endl;
    for(int j = i+1; j < imgPaths.size(); j++)
    {
      compareTwoImages(imgPaths[i], imgPaths[j]);
    }
  }
}

void compareTwoImages(string imageOnePath, string imageTwoPath)
{
  using namespace cv::img_hash;

  //disable opencl acceleration may(or may not) boost up speed of img_hash
  ocl::setUseOpenCL(false);

  //cout << "ColorMomentHash: ";
  compute(ColorMomentHash::create(), imageOnePath, imageTwoPath);
}

bool filterFileName(string fileName)
{
  string extensionList [6] = {".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG"};
  string::size_type n;

  for(int i = 0; i < 6; i++)
  {
    n = fileName.find(extensionList[i]);
  
    if(n != string::npos)
    {
      return true;
    }
  }

  return false;
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

//void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, int argc, char** argv)
void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, string imgOnePath, string imgTwoPath)
{
  
    Mat input = imread(imgOnePath.c_str(), 1);
    if(!input.data)
    {
      cout << "No Image Data" << endl;
      return;
    }

    Mat similarImg = imread(imgTwoPath.c_str(), 1);
    if(!similarImg.data)
    {
      cout << "No Image Data" << endl;
      return;
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
