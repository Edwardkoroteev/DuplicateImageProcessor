# MAKEFILE EXAMPLE USED FROM kevinhughes27 AND WAS MODIFIED
# https://gist.github.com/kevinhughes27/5311609

CC = g++
CFLAGS = -g -Wall -std=c++11
SRCS = main.cpp
PROG = output

OPENCV = `pkg-config --cflags --libs opencv`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(LIBS) $(CFLAGS) $(SRCS) -o $(PROG)
