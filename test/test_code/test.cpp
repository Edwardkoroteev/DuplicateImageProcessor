#include <iomanip>
#include "../../src/imageDuplicateProcess.h"

bool isDirTest();
bool getPathNameTest();
bool filterFileNameTest();
bool filenameFromPathTest();

int main()
{
  if(isDirTest() == true)
    cout << setw(30) << "--PASS" << endl;
  else
    cout << setw(30) << "--FAIL" << endl;

  if(getPathNameTest() == true)
    cout << setw(24) << "--PASS" << endl;
  else
    cout << setw(24) << "--FAIL" << endl;

  if(filterFileNameTest() == true)
    cout << setw(21) << "--PASS" << endl;
  else
    cout << setw(21) << "--FAIL" << endl;

  if(filenameFromPathTest() == true)
    cout << setw(18) << "--PASS" << endl;
  else
    cout << setw(18) << "--FAIL" << endl;

  return 0;
}

bool isDirTest()
{
  cout << "Testing isDir() function...";

  string path1 = "../directory/";
  string path2 = "../dir1/";
  string path3 = "../test_images/";

  if(isDir(path1) == true)
    return false;

  if(isDir(path2) == true)
    return false;

  if(isDir(path3) == false)
    return false;

  return true;
}

bool getPathNameTest()
{
  cout << "Testing getPathName() function...";

  vector<string>imgPathsJpg;
  vector<string>imgPathsPng;
  
  string path1 = "../directory/";
  string path2 = "../dir1/";
  string path3 = "../test_images/";

  if(getPathName(imgPathsJpg, imgPathsPng, path1) == true)
    return false;
  if(getPathName(imgPathsJpg, imgPathsPng, path2) == true)
    return false;
  if(getPathName(imgPathsJpg, imgPathsPng, path3) == false)
    return false;

  return true;
}

bool filterFileNameTest()
{
  cout << "Testing filterFileName() function...";

  string jpgFiles[4] = {"file1.jpg", "file2.JPG", "file3.jpeg", "file4.JPEG"};
  string pngFiles[2] = {"file1.png", "file2.PNG"};
  string wrongFiles[5] = {"file1.heic", "file2.raw", "file3.pdf", "file4.exe", "file5"};

  for(int i = 0; i < 4; i++)
    if(filterFileName(jpgFiles[i]) != "jpg")
      return false;

  for(int i = 0; i < 2; i++)
    if(filterFileName(pngFiles[i]) != "png")
      return false;
  
  for(int i = 0; i < 5; i++)
    if(filterFileName(wrongFiles[i]) != "none")
      return false;

  return true;
}

bool filenameFromPathTest()
{
  cout << "Testing filenameFromPath() function ...";

  string path1 = "../dir/file1.jpg";
  string path2 = "../dir/file2.jpg";
  string path3 = "../dir/file3.jpg";

  if(filenameFromPath(path1) != "file1.jpg")
    return false;
  if(filenameFromPath(path2) != "file2.jpg")
    return false;
  if(filenameFromPath(path3) != "file3.jpg")
    return false;

  return true;
}
