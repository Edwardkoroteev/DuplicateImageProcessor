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
If there is an error that says *opencv.pc* not found. 
Follow the steps below.

1. Add the following line of code into your `.bash_profile` file.
   1. <pre>`export PKG_CONFIG_PATH="/usr/local/Cellar/opencv/<b>YOUR VERSION</b>/lib/pkgconfig/"`</pre>.
2. Change into directory
   1. <pre>`cd /usr/local/Cellar/opencv/<b>YOUR VERSION</b>/lib/pkgconfig/`</pre>
