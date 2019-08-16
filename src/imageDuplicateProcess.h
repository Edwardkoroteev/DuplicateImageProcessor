/* 
   Copyright (c) 2019 Edwardkoroteev
   This program is released under the MIT License
   Please see the file 'LICENSE' in this distribution for
   license terms.
 */

#ifndef MAIN_HEADER
#define MAIN_HEADER

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

void run();
void computeImages(vector<string>imgPathsJpg, vector<string>imgPathsPng, string pathTo);
bool compute(cv::Ptr<cv::img_hash::ImgHashBase> algo, string imgOnePath, string imgTwoPath);
bool getPathName(vector<string> &imgPathsJpg, vector<string> &imgPathsPng, string path);
string filterFileName(string fileName);
void comparingImages(vector<string> smaller, vector<string> larger, string pathTo);
void compareDiffFormatImg(vector<string> imgPathsPng, vector<string> imgPathsJpg, string pathTo);
bool compareTwoImages(string imageOnePath, string imageTwoPath);
bool ifImageSameDisplay(string imgOnePath, string imgTwoPath, double similarityUnit);
bool isDir(string path);
void comparingSameFormatImage(vector<string> &imgPaths, string pathTo);
int fileSize(string path);
bool moveFile(string from, string to);
string filenameFromPath(string path);
bool confirmPath(string message, string &path);

#endif
