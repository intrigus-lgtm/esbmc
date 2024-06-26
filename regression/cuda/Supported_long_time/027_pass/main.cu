#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime_api.h>
#include <assert.h>

#define N 2	//512

__global__ void Asum(int *a, int *b, int *c){
	c[threadIdx.x] = a[threadIdx.x] + b[threadIdx.x];
}

int main(void){
	int *a, *b, *c;
	int *dev_a, *dev_b, *dev_c;
	int size = N*sizeof(int);

	cudaMalloc((void**)&dev_a, size);
	cudaMalloc((void**)&dev_b, size);
	cudaMalloc((void**)&dev_c,size);

	a = (int*)malloc(size);
	b = (int*)malloc(size);
	c = (int*)malloc(size);

	for (int i = 0; i < N; i++)
		a[i] = 10;

	for (int i = 0; i < N; i++)
		b[i] = 10;

	cudaMemcpy(dev_a,a,size, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_b,b,size, cudaMemcpyHostToDevice);

	//Asum<<<1,N>>>(dev_a,dev_b,dev_c);
	ESBMC_verify_kernel(Asum,1,N,dev_a,dev_b,dev_c);

	cudaMemcpy(c,dev_c,size,cudaMemcpyDeviceToHost);

	for (int i = 0; i < N; i++){
		assert(c[i]==a[i]+b[i]);
	}

	free(a); free(b); free(c);

	cudaFree(dev_a);
	cudaFree(dev_c);
	cudaFree(dev_b);

	return 0;
}
