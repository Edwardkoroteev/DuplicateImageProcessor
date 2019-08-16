#  Copyright (c) 2019 Edwardkoroteev
#  This program is released under the MIT License
#  Please see the file 'LICENSE' in this distribution for
#  license terms.

CC = g++
CFLAGS = -g -Wall -std=c++17
SRCS = src/main.cpp src/imageDuplicateProcess.cpp
PROG = imageDuplicateProcessor

OPENCV = `pkg-config --cflags --libs opencv`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(LIBS) $(CFLAGS) $(SRCS) -o $(PROG)
