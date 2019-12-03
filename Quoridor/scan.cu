 //#include <wb.h>
#include <stdio.h> 
extern "C"{
	#include "aiMinMax.h"
	#include "scan.h" 
};

 #define block_size 512 //@@ you can change this
  
 /*#define wbchecestmt) \
 do { \
 cudaerror_t err = stmt; \
 if (err != cudasuccess) { \
 wblog(error, "failed to run stmt ", #stmt); \
 return -1; \
 } \
 } while (0)*/

 __global__ void fixup(float *input, float *aux, int len) {
    unsigned int t = threadIdx.x, start = 2 * blockIdx.x * block_size;
    if (blockIdx.x) {
       if (start + t < len)
         input[start + t] += aux[blockIdx.x - 1];//these are also add lines
       if (start + block_size + t < len)
         input[start + block_size + t] += aux[blockIdx.x - 1];//these are also add lines
    }
 }

 __global__ void scan(float *input, float *output, float *aux, int len) {
 // load a segment of the input vector into shared memory
   __shared__ float scan_array[block_size << 1];
   unsigned int t = threadIdx.x, start = 2 * blockIdx.x * block_size;
   if (start + t < len)
     scan_array[t] = input[start + t];
   else
     scan_array[t] = 0;
   if (start + block_size + t < len)
      scan_array[block_size + t] = input[start + block_size + t];
   else
     scan_array[block_size + t] = 0;
   __syncthreads();
 
   // reduction
   int stride;
   for (stride = 1; stride <= block_size; stride <<= 1) {
      int index = (t + 1) * stride * 2 - 1;
      if (index < 2 * block_size)
        scan_array[index] += scan_array[index - stride];//this line is the add line
      __syncthreads();
   }

   // post reduction
   for (stride = block_size >> 1; stride; stride >>= 1) {
      int index = (t + 1) * stride * 2 - 1;
      if (index + stride < 2 * block_size)
        scan_array[index + stride] += scan_array[index];//this line is the add line
      __syncthreads();
   }

   if (start + t < len)
     output[start + t] = scan_array[t];
   if (start + block_size + t < len)
     output[start + block_size + t] = scan_array[block_size + t];

   if (aux && t == 0)
      aux[blockIdx.x] = scan_array[2 * block_size - 1];
   }

int takeVal(Value val) {
   //wbarg_t args;
   float *hostinput; // the input 1d list
   float *hostoutput; // the output list
   float *deviceinput;
   float *deviceoutput;
   float *deviceauxarray, *deviceauxscannedarray;
   int numelements=val->size; // number of elements in the list

   //args = wbarg_read(argc, argv);

   //wbtime_start(generic, "importing data and creating memory on host");
   
   float hostarr[numelements];
   for(int i=0;i<numelements;i++){
 	hostarr[i] = val->v;
	printf("hostarr[%d] = %f",i,hostarr[i]);
   }
   hostinput = hostarr;

 //(float *)wbimport(wbarg_getinputfile(args, 0), &numelements);//make this call a method from sequential ai that generates a list
   cudaHostAlloc(&hostoutput, numelements * sizeof(float), cudaHostAllocDefault);
   //wbtime_stop(generic, "importing data and creating memory on host");

   //wblog(trace, "the number of input elements in the input is ", numelements);

   //wbtime_start(gpu, "allocating gpu memory.");

   cudaMalloc((void **)&deviceinput, numelements * sizeof(float));
   cudaMalloc((void **)&deviceoutput, numelements * sizeof(float));

   // xxx the size is fixed for ease of implementation.
   cudaMalloc(&deviceauxarray, (block_size << 1) * sizeof(float));
   cudaMalloc(&deviceauxscannedarray, (block_size << 1) * sizeof(float));
   //wbtime_stop(gpu, "allocating gpu memory.");

   //wbtime_start(gpu, "clearing output memory.");
   cudaMemset(deviceoutput, 0, numelements * sizeof(float));
   //wbtime_stop(gpu, "clearing output memory.");

    //wbtime_start(gpu, "copying input memory to the gpu.");
    cudaMemcpy(deviceinput, hostinput, numelements * sizeof(float),cudaMemcpyHostToDevice);
    //wbtime_stop(gpu, "copying input memory to the gpu.");

    //@@ initialize the grid and block dimensions here
    int numblocks = ceil((float)numelements / (block_size << 1));
    dim3 dimgrid(numblocks, 1, 1);
    dim3 dimblock(block_size, 1, 1);
    //wblog(trace, "the number of blocks is ", numblocks);

    //wbtime_start(compute, "performing cuda computation");
    //@@ modify this to complete the functionality of the scan
    //@@ on the deivce
    scan<<<dimgrid, dimblock>>>(deviceinput, deviceoutput, deviceauxarray, numelements);
    cudaDeviceSynchronize();
    scan<<<dim3(1, 1, 1), dimblock>>>(deviceauxarray, deviceauxscannedarray, NULL, block_size << 1);
    cudaDeviceSynchronize();
    fixup<<<dimgrid, dimblock>>>(deviceoutput, deviceauxscannedarray, numelements);

    cudaDeviceSynchronize();
    //wbtime_stop(compute, "performing cuda computation");

    //wbtime_start(copy, "copying output memory to the cpu");
    cudaMemcpy(hostoutput, deviceoutput, numelements * sizeof(float),
    cudaMemcpyDeviceToHost);
    //wbtime_stop(copy, "copying output memory to the cpu");

    //wbtime_start(gpu, "freeing gpu memory");
    cudaFree(deviceinput);
    cudaFree(deviceoutput);
    cudaFree(deviceauxarray);
    cudaFree(deviceauxscannedarray);
    //wbtime_stop(gpu, "freeing gpu memory");

    //wbsolution(args, hostoutput, numelements);
    int ret = *((int*)(&hostinput));
    free(hostinput);
    cudaFreeHost(hostoutput);

    return ret;
 }
