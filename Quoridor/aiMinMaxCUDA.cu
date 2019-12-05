//#include <wb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"{
	#include "setup.h"
	#include "play.h"
	#include "globals.h"
	#include "aiMinMax.h"
}

#define thread_size 175
#define block_size 512
#define size 9
#define max_moves 175 //max leegs with a little extra

//how scancode relates: just do 1 pass down to calc terminals then 1 pass to calc min or max 
// in first pass should just stride loop
// each stride should be maxLegals^treedepth (passes are different stride locations) 
//if invalid option, set value to unfavorable
//look at inefficient work kernel (just slap something together)

//leegs is boardstate, depth is treedepth, minning and maxing are whether we are running min or max, finalZero, finalOne and finalOri are returned row, col and ori
__global__ void treeGen(int row, int col, char* color, char** walltrack, int depth, int minOrMax, int* array){

	//1 is white, 0 is black

	
	int t = blockIdx.x*blockDim.x+threadIdx.x;
        //determine whether a legal has kids (pawns at winning or depth = 0) this for loop is here because inefficient scan kernel
        for(int stride = 1;stride<=threadIdx.x;stride *= depth){
                //go through legalactions here
                __syncthreads();
                
                //how to assign min and max layers?
                //what to return? how to return top move? go back up tree minning and maxing
                //how to go back up tree? reduction kernel type thing

		//too much of this kernel relies on sequential method calls. I don't know if it will work
                
                if(t<max_moves){
                        float * leegs = getLegalMoves(size,walltrack,color,row,col);
			int decider = (int)(*(leegs+t)+0);
                        	if((strcmp(color,"black")==0 && row == 0) || (strcmp(color,"white")==0 && row == size-1)){
                        		array[t] = 1000;
                        	}
				int minpathw=size*size, minpathb=size*size;
                        	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
                        	if(minpathw<minpathb && strcmp("white",color)==0){//these will trigger early
                        		array[t] = 500;
                        	}
                        	if(minpathb<minpathw && color==0){
                        		array[t] = 500;
                        	}
                        	if((decider<0) && minOrMax==1){
                        		array[t] = -999;
                        	}
                        	if((decider<0) && minOrMax==0){
                        		array[t] = 999;
                        	}
                        	if((!decider<0) && t<6){
                        		int newrow = (int)(*(leegs+t)+0);
                        		int newcol = (int)(*(leegs+t)+1);
                        		leegs = getLegalMoves(size,walltrack,color,newrow,newcol);
                        	}
                        	if((!decider<0) && t<87  && t>5){
                        		walltrack[(int)(*(leegs+t)+0)][(int)(*(leegs+t)+1)] = 'H';
                        		leegs = getLegalMoves(size,walltrack,color,row,col);
                        	}
                        	if((!leegs[t]<0) && t<172 && t>86){
                        		walltrack[(int)(*(leegs+t)+0)][(int)(*(leegs+t)+1)] = 'V';
                        		leegs = getLegalMoves(size,walltrack,color,row,col);
                        	}
                        	
                        }
	
                              // reduction
			   int stride;
			   for (stride = 1; stride <= block_size; stride <<= 1) {
			      int index = (t + 1) * stride * 2 - 1;
			      if (index < 2 * block_size){
			      	if(stride%(depth)==0){
			        //compare instead of add
			        array[index] = min(array[index],array[index-stride]);//how to get 173 vals? like this?
			      __syncthreads();
			      }
			      else{
			      	array[index] = max(array[index],array[index-stride]);//how to get 173 vals?
			      __syncthreads();
			      }
			   }
			} 
}
}

