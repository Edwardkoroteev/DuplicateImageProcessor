#include <iostream>
#include <dirent.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/img_hash.hpp>

using namespace cv;
using namespace std;

void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, int argc, char** argv);
void readImg(int argc, char** argv);
void getFileName();
bool filterFileName(string fileName);

int main(int argc, char** argv)
{
  //getFileName();
  if(argc != 3)
  {
    printf("usage: DisplayImage.out <./testImg.png>\n");
    return -1;
  }

  using namespace cv::img_hash;


  //disable opencl acceleration may(or may not) boost up speed of img_hash
  ocl::setUseOpenCL(false);

  //cout << "ColorMomentHash: ";
  compute(ColorMomentHash::create(), argc, argv);

  return 0;
}

void getFileName()
{
  string start;
  DIR *dir;
  struct dirent *ent;

  cout << "Enter the directory path: ";
  cin >> start;

  start = "/Users/kedward/Desktop/DuplicateImageProcessor/test";

  if ((dir = opendir(start.c_str())) != NULL)
  {
    while((ent = readdir(dir)) != NULL)
    {
      if(filterFileName(ent->d_name) != true)
      {
        cout << start + ent->d_name << endl;
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

bool filterFileName(string fileName)
{
  string extensionList [4] = {".png", ".PNG", ".jpg", ".JPG"};
  string::size_type n;

  for(int i = 0; i < 4; i++)
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

    waitKey(0);
}

void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, int argc, char** argv)
{
  
    Mat input = imread(argv[1]);

    if(!input.data)
    {
      cout << "No Image Data" << endl;
      return;
    }

    imshow("Display Image", input);

    Mat similarImg;

    similarImg = imread(argv[2], 1);

    Mat hashInput;
    Mat hashSimilar;

    algo->compute(input, hashInput);
    algo->compute(similarImg, hashSimilar);

    double test = algo->compare(hashInput, hashSimilar);

    if(test < 0.4 && test >= 0)
    {
      cout << "Images are the same" << endl;
    }
    else
    {
      cout << "Not the same" << endl;
    }

    cout << "input: " << argv[1] << endl;
    cout << "similarImg" << argv[2] << endl;
    imshow("input", input);
    imshow("similarImg", similarImg);
    waitKey(0);

}
