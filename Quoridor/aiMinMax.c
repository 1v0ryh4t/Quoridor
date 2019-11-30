#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include <string.h>
#include "play.h"
#include "globals.h"
#include "ai.h"

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

//TODO: add eval func. wire into decision
//These will be kernels NOTE: 'move' just tracks whether we are simulating white or black
int minVal(int size, char** walltrack, int depth, char* color, int row, int col){
	//int maxVal(int size, char** walltrack, int depth, char* color, int row, int col);//function prototype, ignore
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = 9999;//supposed to be inf
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {
			int evalWhite = 0;//10+(minpathb-minpathw)+(white.walls-black.walls+1)/(minpathw);//white
			evalWhite = minpathw;//scratch eval func update: seems to work? fingers crossed
			return evalWhite;
		}
		for (int i=0;i<6;i++) {//move code
			getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, white.row, white.col, ' ');//Do I even Need historyUpdate? A: YES! Don't delete!
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
			return evalBlack;
		}
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
			if(possiblemoves[i][0]!=-1){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate simulates nextmove?
				int u = maxVal(size, walltrack, depth-1, "white", white.row, white.col);//stores maxval so that debugger will go into maxval
				if(v>u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
			}
		}
	}
	return v;
}


int maxVal(int size, char** walltrack, int depth, char* color, int row, int col){
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = -9999;
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {
			int evalWhite = 0;//10+(minpathb-minpathw)+(white.walls-black.walls+1)/(minpathw);//white
			evalWhite = minpathw;
			return evalWhite;
		}
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, white.row, white.col, ' ');//hostoryupdate simulates nextmove?
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
				//printf("white moves to %d, %d\n",white.row, white.col);
				int currVal = -1*(minVal(size, walltrack, depth, color, white.row, white.col));
				//printf("currVal = %d\n",currVal);
				if(currVal > bestVal){
					bestVal = currVal;
					bestrow = white.row;
					bestcol = white.col;
					bestori = ' ';
				}//else{undo(&history, walltrack, size, 1);}
				//undo(&history, walltrack, size, 1);
				undoCounter++;
			}
			//undo(&history, walltrack, size, 1);
		}
		//printhistory(&history);
		//undo(&history, walltrack, size, 1);
		//printf("\n");
		//printhistory(&history);
	}
	else {
		getvalidmoves(size, walltrack, color, black.row, black.col);
		int pawnmoves[6][2];
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
	undo(&history, walltrack, size, undoCounter-1);
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int oldMinpathw = minpathw, oldMinpathb = minpathb;
	if (strcmp(color, "white")==0) {
			for (int i=1;i<=size;i++) {//wall code
				for (int j=0;j<=size-1;j++) {
					minpathsMinMax(size, walltrack, &minpathw, &minpathb);
					if (black.pathing[i-1][j]>5) {
						continue;
					}
					if (isvalidwall(walltrack, 'H', i, j, size, "white")) {
						walltrack[i][j]='H';
						historyupdate(&history, color, i, j, 'H');
						minpathsMinMax(size, walltrack, &minpathw, &minpathb);
						//should be a sheilding clause here. only run if black's minpath increases and whites minpath doesn't???
						if(oldMinpathb < minpathb && minpathb<minpathw/*&& bestVal < (-1*(oldMinpathb-size/2))*/){
							bestrow = i;
							bestcol = j;
							bestori = 'H';
						}
						undo(&history, walltrack, size, 1);
					}
				}//NOTE: need to undo??
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
						if(oldMinpathb < minpathb && minpathb<minpathw /*&& bestVal < (-1*(oldMinpathb-size/2))*/){
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
						if(oldMinpathw < minpathw && minpathw<minpathb /*&& bestVal < (-1*(oldMinpathw-size/2))*/){
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
						if(oldMinpathw < minpathw && minpathw<minpathb /*&& bestVal < (-1*(oldMinpathw-size/2))*/){
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


/**
 * The following is python code on Minimax that will come in handy later
 *
 * self.bestMove = -1000
        """each of these decrements depth"""

        def minVal(gameState, agentIndex, depth):
            #if terminal test(state) then return utility(state)
            if(gameState.isWin() or gameState.isLose() or depth == 0):
                return self.evaluationFunction(gameState)
            v = float("inf")
            # increment from 0 to getNumAgents
            for act in gameState.getLegalActions(agentIndex):
                if(agentIndex == gameState.getNumAgents()-1):
                    v = min(v, maxVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth-1))
                else:
                    v = min(v, minVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth))
            #print "min ", v
            return v

        def maxVal(gameState, agentIndex, depth):
            #if terminal test(state) then return utility(state)
            if(gameState.isWin() or gameState.isLose() or depth == 0):
                return self.evaluationFunction(gameState)
            v = float("inf")*-1
            for act in gameState.getLegalActions():
                v = max(v, minVal(gameState.generateSuccessor(0, act),1,depth))
            #print "max ", v
            return v

        def minDecision(gameState):
            #returns the Alpha in Actions(state) maximizing the min(result(alpha,state))
            self.bestMove = Directions.EAST
            bestVal = float('-inf')
            for act in gameState.getLegalActions(0):
                succState = gameState.generateSuccessor(0, act)

                currentValue = minVal(succState,1, self.depth)
                if(currentValue > bestVal):
                    bestVal = currentValue
                    self.bestMove = act

            return self.bestMove
        return minDecision(gameState)
 */
