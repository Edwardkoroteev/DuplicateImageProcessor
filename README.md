# DuplicateImageProcessor
Searches a directory to find if there are duplicate images

# Installation
###### Installing Homebrew
`/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
###### Installing OpenCV
`brew install opencv`
###### Install pkg-config
`brew install pkg-config`
###### Check OpenCV linker flags
`pkg-config --cflags --libs opencv`

**If an error occurs**
1. Add the following line of code into your `.bash_profile` file.
   1. `export PKG_CONFIG_PATH="/usr/local/Cellar/opencv/YOUR_VERSION/lib/pkgconfig/"`.
2. Change into directory.
   1. `cd /usr/local/Cellar/opencv/YOUR_VERSION/lib/pkgconfig/`.
3. Make sure an `opencv.pc` file is there. If it is similar but named differently. Rename the file to `opencv.pc`.
4. Log out. Log in to reset `.bash_history`.
