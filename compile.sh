#!/bin/sh
# gcc -c facetrain.c 
# gcc -c imagenet.c 
# gcc -c pgmimage.c 
# gcc -c backprop.c 
# gcc backprop.o imagenet.o facetrain.o pgmimage.o -lm -o main

# ./main

nvcc -c kernel.cu -o kernel.o

gcc -c  facetrain.c -o facetrain.o
gcc -c  backprop.c -o backprop.o
gcc -c  imagenet.c -o imagenet.o
gcc -c  pgmimage.c -o pgmimage.o

nvcc  facetrain.o backprop.o imagenet.o pgmimage.o kernel.o -o main -lm

