/* aiMinMaxCUDA.h
 *
 *  Created on: Nov 18, 2019
 *      Author: tanner
 */

#ifndef AIMINMAXCUDA_H_
#define AIMINMAXCUDA_H_

void minpathsMinMax(int size, char** walltrack, int* minpathw, int* minpathb);

int minVal(int size, char** walltrack, int depth, char* color, int row, int col, int counter);

int maxVal(int size, char** walltrack, int depth, char* color, int row, int col, int counter);

void minMaxDecision(int size, char** walltrack, int depth, char* color, int*chosenr, int* chosenc, char* chosenori);

#endif /* AIMINMAXCUDA_H_ */

