#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include <string.h>
#include "play.h"
#include "globals.h"
#include "aiMinMax.h"

//is this neccesary?? YES?
void minpathsMinMax(int size, char **walltrack, int *minpathw, int *minpathb) {
	getpath(size, walltrack);
	*minpathw=white.minpath;
	*minpathb=black.minpath;
	/*if (*minpathw==0) {// is this logic even neccesary?
		*minpathw=1;
		*minpathb=size*size;
	}
	else if (*minpathb==0) {
		*minpathb=1;
		*minpathw=size*size;
	}*/
	return;
}


//returns a list of valid moves and wall placements for a color
float*  getLegalMoves(int size, char** walltrack, char* color, int row, int col){
	int legalints[172][2];

	for (int i=0;i<172;i++) {
		for (int j=0;j<2;j++) {
			legalints[i][j] = -2;//reset legalints
		}
	}

	if(strcmp(color, "white")==0){
		int counter = 0;//counter so that I can compress a 9x9 into an 24x2
		getvalidmoves(size, walltrack, color, white.row, white.col);
		for (int i=0;i<6;i++) {
			for (int j=0;j<2;j++) {
				legalints[i][j] = possiblemoves[i][j];//copies to temp array for safekeeping
				//printf("legalints[%d][%d]=%d\n",i,j,legalints[i][j]);
			}
		}
		for (int i=1;i<=size;i++) {
			for (int j=0;j<=size-1;j++) {
				int index = i+j+counter+5;
				while(legalints[index-1][0]<-1){index-=1;}
				if (isvalidwall(walltrack, 'H', i, j, size, "white")) {
					//add to array. How to represent H/V? Array placement
					//how to not skip indices? check 1 before
					historyupdate(&history, color, i, j, 'H');
					legalints[index][0] = i;
					legalints[index][1] = j;
					//printf("hlegalints[%d]=%d,%d\n",index,legalints[index][0],legalints[index][1]);

					undo(&history, walltrack, size, 1);
				}else{
					legalints[index][0] = -1;
					legalints[index][1] = -1;
					//printf("hlegalints[%d]=%d,%d\n",index,legalints[index][0],legalints[index][1]);
				}
				if(j==size-1 && i<size){
					counter+=j;
				}
				if(j==size-1 && i==size){
					counter=index;
				}
			}
		}
		for (int i=1;i<=size;i++) {
			for (int j=0;j<=size-1;j++) {
				int index = i+j+counter+5;
				while(legalints[index-1][0]<-1){index-=1;}
				if (isvalidwall(walltrack, 'V', i, j, size, "white")) {
					//add to array. How to represent H/V? Array placement
					//how to not skip indices? check 1 before
					historyupdate(&history, color, i, j, 'V');
					legalints[index][0] = i;
					legalints[index][1] = j;
					//printf("vlegalints[%d]=%d,%d\n",index,legalints[index][0],legalints[index][1]);

					undo(&history, walltrack, size, 1);
				}else{
					legalints[index][0] = -1;
					legalints[index][1] = -1;
					//printf("vlegalints[%d]=%d,%d\n",index,legalints[index][0],legalints[index][1]);
				}
				if(j==size-1){
					counter+=j;
				}
			}
		}
	}else{
		int counter = 0;//counter so that I can compress a 9x9 into an 24x2
		getvalidmoves(size, walltrack, color, black.row, black.col);
		for (int i=0;i<6;i++) {
			for (int j=0;j<2;j++) {
				legalints[i][j] = possiblemoves[i][j];//copies to temp array for safekeeping
				//printf("legalints[%d]=%d,%d\n",i,legalints[i][0],legalints[i][0]);
			}
		}
		for (int i=1;i<=size;i++) {
			for (int j=0;j<=size-1;j++) {
				int index = i+j+counter+5;
				while(legalints[index-1][0]<-1){index-=1;}
				if (isvalidwall(walltrack, 'H', i, j, size, "black")) {
					//add to array. How to represent H/V? Array placement
					//how to not skip indices? check 1 before
					historyupdate(&history, color, i, j, 'H');
					legalints[index][0] = i;
					legalints[index][1] = j;
					//printf("hlegalints[%d]=%d,%d\n",index,legalints[index][0],legalints[index][1]);

					undo(&history, walltrack, size, 1);
				}
				else{
					legalints[index][0] = -1;
					legalints[index][1] = -1;
					//printf("hlegalints[%d]=%d,%d\n",index,legalints[index][0],legalints[index][1]);
				}
				if(j==size-1 && i<size){
					counter+=j;
				}
				if(j==size-1 && i==size){
					counter=index;
				}
			}
		}
		for (int i=1;i<=size;i++) {
			for (int j=0;j<=size-1;j++) {
				int index = i+j+counter+5;
				while(legalints[index-1][0]<-1){index-=1;}
				if (isvalidwall(walltrack, 'V', i, j, size, "black")) {
					//add to array. How to represent H/V? Array placement
					//how to not skip indices? check 1 before
					historyupdate(&history, color, i, j, 'V');
					legalints[index][0] = i;
					legalints[index][1] = j;
					//printf("vlegalints[%d][0]=%d,%d\n",index,legalints[index][0],legalints[index][1]);

					undo(&history, walltrack, size, 1);
				}
				else{
					legalints[index][0] = -1;
					legalints[index][1] = -1;
					//printf("vlegalints[%d]=%d,%d\n",index,legalints[index][0],legalints[index][1]);
				}
				if(j==size-1){
					counter+=j;
				}
			}
		}
	}
	for(int i=0; i<172;i++){
		for(int j=0;j<2;j++){
			legals[i][j] = legalints[i][j];
			printf("legals[%d][%d] == %f\n",i,j,legals[i][j]);
		}
	}
	return legals;
}

//TODO: add eval func. wire into decision
//These will be kernels NOTE: 'move' just tracks whether we are simulating white or black
int minVal(int size, char** walltrack, int depth, char* color, int row, int col){
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = 9999;//supposed to be inf
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {
			//printf("minpath counter = %d\n",counter);
			return minpathw;
		}
		for (int i=0;i<6;i++) {//move code
			getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, white.row, white.col, ' ');
				//printf("add a branch\n");
				int u = maxVal(size, walltrack, depth-1, "black", black.row, black.col);
				if(v>u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
			}
		}
	}
	else {
		getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {
			int evalBlack = 0;//10+(minpathw-minpathb)+(black.walls-white.walls+1)/(minpathb);//black
			evalBlack = minpathb;//scratch eval func
			//printf("minpath counter = %d\n",counter);
			return evalBlack;
		}
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
			if(possiblemoves[i][0]!=-1){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate simulates nextmove?
				//printf("add a branch\n");
				int u = maxVal(size, walltrack, depth-1, "white", white.row, white.col);//stores maxval so that debugger will go into maxval
				if(v>u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
			}
		}
	}
	//printf("final counter = %d\n",counter);
	return v;
}


int maxVal(int size, char** walltrack, int depth, char* color, int row, int col){
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = -9999;
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {
			int evalWhite = 0;
			evalWhite = minpathw;
			//printf("counter = %d\n");
			return evalWhite;
		}
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, white.row, white.col, ' ');
				int u = minVal(size, walltrack, depth-1, "black", black.row, black.col);
				if(v<u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
			}
		}
	}
	else {//black
		getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {//NOTE: switching black and white evals stops the code
			int evalBlack = 0;//10+(minpathw-minpathb)+(black.walls-white.walls+1)/(minpathb);//black
			evalBlack = minpathb;//scratch eval func
			//printf("minpath counter = %d\n");
			return evalBlack;
		}
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
			if(possiblemoves[i][0]!=-1){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, black.row, black.col, ' ');
				int u = minVal(size, walltrack, depth-1, "white", white.row, white.col);
				if(v<u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
			}
		}
	}
	//printf("final counter = %d\n",counter);
	return v;
}

//will be kernelcall. This function chooses chosenr, chosenc, and chosenori
void minMaxDecision(int size, char** walltrack, int depth, char* color, int*chosenr, int* chosenc, char* chosenori) {
	int bestVal = -9999;
	int bestrow = -1;
	int bestcol = -1;
	char bestori = 'c';
	int undoCounter = 0;
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);
		getLegalMoves(size, walltrack, color, white.row, white.col);
		printf("address of leegs %f\n",legals[0][0]);//should be 1
		int pawnmoves[6][2];
		for (int i=0;i<6;i++) {
			for (int j=0;j<2;j++) {
				pawnmoves[i][j] = possiblemoves[i][j];//copies to temp array for safekeeping
			}
		}
		for (int i=0;i<6;i++) {
			if (pawnmoves[i][0]!=-1) {
				white.row=pawnmoves[i][0];//simulate nextMove
				white.col=pawnmoves[i][1];//simulate nextMove
				historyupdate(&history, color, white.row, white.col, ' ');
				printf("white moves to %d,%d\n",white.row,white.col);
				int currVal = -1*(minVal(size, walltrack, depth, color, white.row, white.col));
				if(currVal > bestVal){
					bestVal = currVal;
					bestrow = white.row;
					bestcol = white.col;
					bestori = ' ';
				}
				undoCounter++;
			}
		}
	}
	else {
		getvalidmoves(size, walltrack, color, black.row, black.col);
		getLegalMoves(size, walltrack, color, black.row, black.col);
		printf("address of leegs %f\n",legals[0][0]);//should be 1
		int pawnmoves[6][2];
		return;
		for (int i=0;i<6;i++) {
			for (int j=0;j<2;j++) {
				pawnmoves[i][j] = possiblemoves[i][j];//copies to temp array for safekeeping
			}
		}
		for (int i=0;i<6;i++) {
			if (pawnmoves[i][0]!=-1) {
				black.row=pawnmoves[i][0];//simulate nextMove
				black.col=pawnmoves[i][1];//simulate nextMove
				historyupdate(&history, color, black.row, black.col, ' ');
				//printf("black moves to %d, %d\n",black.row, black.col);
				int currVal = -1*(minVal(size, walltrack, depth, color, black.row, black.col));
				//printf("currVal = %d\n",currVal);
				if(currVal > bestVal){
					bestVal = currVal;
					bestrow = black.row;
					bestcol = black.col;
					bestori = ' ';
				}//else{undo(&history, walltrack, size, 1);}
				undoCounter++;
			}
		}
	}
	printf("add %d branches\n",undoCounter);
	undo(&history, walltrack, size, undoCounter-1);//undoes move NOTE: bestvars unchanged currently
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);//update minpaths before placing walls
	int oldMinpathw = minpathw, oldMinpathb = minpathb;
	if (strcmp(color, "white")==0) {
			for (int i=1;i<=size;i++) {
				for (int j=0;j<=size-1;j++) {
					minpathsMinMax(size, walltrack, &minpathw, &minpathb);
					if (black.pathing[i-1][j]>5) {
						continue;
					}
					if (isvalidwall(walltrack, 'H', i, j, size, "white")) {
						walltrack[i][j]='H';
						historyupdate(&history, color, i, j, 'H');
						minpathsMinMax(size, walltrack, &minpathw, &minpathb);
						if(oldMinpathb < minpathb && (minpathb<minpathw || minpathb == 1)){
							bestrow = i;
							bestcol = j;
							bestori = 'H';
						}
						undo(&history, walltrack, size, 1);
					}
				}
			}
			for (int i=1;i<=size;i++) {//wall code
				for (int j=0;j<=size-1;j++) {
					minpathsMinMax(size, walltrack, &minpathw, &minpathb);
					if (black.pathing[i-1][j]>5) {
						continue;
					}
					if (isvalidwall(walltrack, 'V', i, j, size, "white")) {
						walltrack[i][j]='V';
						historyupdate(&history, color, i, j, 'V');
						minpathsMinMax(size, walltrack, &minpathw, &minpathb);
						//should be a sheilding clause here. if minpath your color increases and my minpath doesn't?
						if(oldMinpathb < minpathb && (minpathb<minpathw || minpathb == 1)){
							bestrow = i;
							bestcol = j;
							bestori = 'V';
						}
						undo(&history, walltrack, size, 1);
					}
				}
			}
			undo(&history, walltrack, size, 1);
		}
		if (strcmp(color, "black")==0) {
			for (int i=1;i<=size;i++) {//wall code
				for (int j=0;j<=size-1;j++) {
					minpathsMinMax(size, walltrack, &minpathw, &minpathb);
					if (white.pathing[i-1][j]>5) {
						continue;
					}
					if (isvalidwall(walltrack, 'H', i, j, size, "black")) {
						walltrack[i][j]='H';
						historyupdate(&history, color, i, j, 'H');
						minpathsMinMax(size, walltrack, &minpathw, &minpathb);
						if(oldMinpathw < minpathw && (minpathw<minpathb || minpathw == 1)){
							bestrow = i;
							bestcol = j;
							bestori = 'H';
						}
						undo(&history, walltrack, size, 1);
					}
				}
			}
			for (int i=1;i<=size;i++) {//wall code
				for (int j=0;j<=size-1;j++) {
					minpathsMinMax(size, walltrack, &minpathw, &minpathb);
					if (white.pathing[i-1][j]>5) {
						continue;
					}
					if (isvalidwall(walltrack, 'V', i, j, size, "black")) {
						walltrack[i][j]='V';
						historyupdate(&history, color, i, j, 'V');
						minpathsMinMax(size, walltrack, &minpathw, &minpathb);
						if(oldMinpathw < minpathw && (minpathw<minpathb || minpathw == 1)){
							bestrow = i;
							bestcol = j;
							bestori = 'V';
						}
						undo(&history, walltrack, size, 1);
					}
				}
			}
			undo(&history, walltrack, size, 1);
		}
	*chosenr = bestrow;
	*chosenc = bestcol;
	*chosenori = bestori;
	//printf("row = %d | col = %d | ori = %c\n",bestrow,bestcol,bestori);
	return;
}

