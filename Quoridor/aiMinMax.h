/*created on: Nov 18, 2019
 *      Author: tanner
 */

#ifndef AIMINMAX_H_
#define AIMINMAX_H_

float legals[172][2];//legalmoves of any boardstate

float* getLegalMoves(int size, char** walltrack, char* color, int row, int col);

void minpathsMinMax(int size, char** walltrack, int* minpathw, int* minpathb);

int minVal(int size, char** walltrack, int depth, char* color, int row, int col);

int maxVal(int size, char** walltrack, int depth, char* color, int row, int col);

void minMaxDecision(int size, char** walltrack, int depth, char* color, int*chosenr, int* chosenc, char* chosenori);

#endif /* AIMINMAX_H_ */

