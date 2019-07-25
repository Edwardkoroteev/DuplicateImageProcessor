#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/img_hash.hpp>

using namespace cv;
using namespace std;

void compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, int argc, char** argv);
void readImg(int argc, char** argv);

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    printf("usage: DisplayImage.out <./testImg.png>\n");
    return -1;
  }

  using namespace cv::img_hash;


  //disable opencl acceleration may(or may not) boost up speed of img_hash
  ocl::setUseOpenCL(false);

  cout << "ColorMomentHash: ";
  compute(ColorMomentHash::create(), argc, argv);
  
  return 0;
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

    cout << "Data: " << algo->compare(hashInput, hashSimilar) << endl;
}
