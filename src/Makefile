CC = g++
CFLAGS = -g -Wall -std=c++11
SRCS = main.cpp
PROG = run

OPENCV = 'pkg-config --cflags --libs opencv'
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(SRCS) $(LIBS)
