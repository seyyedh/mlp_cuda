#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>
#include "kernel.h"
// #include "kernel.cu"
// #include "dev_array.h"
#include <math.h>

using namespace std;

__global__ void matrixMultiplicationKernel(double* A, double* B, double* C, int N) {

    int ROW = blockIdx.y*blockDim.y+threadIdx.y;
    int COL = blockIdx.x*blockDim.x+threadIdx.x;

    float tmpSum = 0;

    if (ROW < N && COL < N) {
        // each thread computes one element of the block sub-matrix
        for (int i = 0; i < N; i++) {
            tmpSum += A[ROW * N + i] * B[i * N + COL];
        }
    }
    C[ROW * N + COL] = tmpSum;
}


extern "C" void multiplication_GPU(double *l1, double *l2, double **conn, int n1, int n2);
void multiplication_GPU(double *l1, double *l2, double **conn, int n1, int n2)
{

	int N = 1024;
    int SIZE = N*N;

	int input = n1;
	int hiddenlayer = n2;
	
	int inputSize = input * sizeof(double);
	int hiddenSize = hiddenlayer * sizeof(double);

	int connection = input * hiddenlayer;
	int connectionSize = connection * sizeof(double);

	double h_3[input*hiddenSize];

	for (int i = 0; i < input; i++)
			for(int j = 0; j < hiddenSize; j++)
					h_3[i*hiddenSize + j] = conn[i][j];


	double *d_input;
	double *d_conn;
	double *d_pp;
	double *d_out;

	cudaMalloc((void **)&d_input, inputSize);
	cudaMalloc((void **)&d_conn, connectionSize);
	cudaMalloc((void **)&d_pp, connectionSize);
	cudaMalloc((void **)&d_out, hiddenSize);

	cudaMemcpy(d_input, l1, inputSize, cudaMemcpyHostToDevice);
	cudaMemcpy(d_conn, h_3, connectionSize, cudaMemcpyHostToDevice);

	matrixMultiplicationKernel<<<hiddenlayer,inputSize>>>(d_pp, d_input, d_conn, hiddenlayer);


	cudaMemcpy(l2, d_out, hiddenSize, cudaMemcpyDeviceToHost);
	cudaFree(d_conn);
	cudaFree(d_input);
	cudaFree(d_pp);
	cudaFree(d_out);

}