#!/bin/sh
gcc -c facetrain.c 
gcc -c imagenet.c 
gcc -c pgmimage.c 
gcc -c backprop.c 
gcc backprop.o imagenet.o facetrain.o pgmimage.o -lm -o main

./main

