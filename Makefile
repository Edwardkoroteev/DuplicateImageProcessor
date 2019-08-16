# MAKEFILE EXAMPLE USED FROM kevinhughes27 AND WAS MODIFIED
# https://gist.github.com/kevinhughes27/5311609

CC = g++
CFLAGS = -g -Wall -std=c++17
SRCS = src/main.cpp src/imageDuplicateProcess.cpp
PROG = imageDuplicateProcessor

OPENCV = `pkg-config --cflags --libs opencv`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(LIBS) $(CFLAGS) $(SRCS) -o $(PROG)
