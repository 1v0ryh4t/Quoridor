#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include <string.h>
#include "play.h"
#include "globals.h"
#include "ai.h"

//is this neccesary??
void minpathsMinMax(int size, char **walltrack, int *minpathw, int *minpathb) {
	getpath(size, walltrack);
	*minpathw=white.minpath;
	*minpathb=black.minpath;
	if (*minpathw==0) {
		*minpathw=1;
		*minpathb=size*size;
	}
	else if (*minpathb==0) {
		*minpathb=1;
		*minpathw=size*size;
	}
	return;
}

//Can be tweaked
float epsilon = 1.0;

//These will be kernels NOTE: 'move' just tracks whether we are simulating white or black
int minVal(int size, char** walltrack, int depth, char* color, int row, int col){
	//printf("min\n");
	int minpathw, minpathb = size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = 9999;//supposed to be inf
	//iterate through all legal actions using possiblemoves
	if (strcmp(color, "white")==0) {
		if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
			int debugnum = 100*((minpathw - minpathb)+epsilon*((black.walls-white.walls)+1))/minpathb;//TODO: kill this var
			//printf("min eval w = %d\n",debugnum);
			return debugnum;
		}
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		for (int i=0;i<6;i++) {
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				historyupdate(&history, color, white.row, white.col, ' ');//Do I even Need historyUpdate?
				//printf("i = %d\n",i); printf("new row = %d\n", white.row); printf("new col = %d\n", white.col);//TODO: kill these
				//mycolor
				int u = maxVal(size, walltrack, depth-1, "black", black.row, black.col);
				//v = min(v, maxVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth-1))
				if(v>u){
				   v=u;//this is total nextstate because we have newPos and running walltrack
				}
				undo(&history, walltrack, size, 1);
				//printf("v = %d\n",v);
			}
			//undo(&history, walltrack, size, 1);
		}
	}
	else {
		if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
			int debugnum = 100*((minpathb - minpathw)+epsilon*((white.walls-black.walls)+1))/minpathw;//TODO: kill this var
			//printf("min eval b = %d\n",debugnum);
			return debugnum;
		}
		getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
		for (int i=0;i<6;i++) {
			if(possiblemoves[i][0]!=-1 && strcmp(color, "black")==0){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate simulates nextmove?
				//printf("i = %d\n",i); printf("new row = %d\n", black.row); printf("new col = %d\n", black.col);//TODO: kill these
				//yourcolor
				int u = maxVal(size, walltrack, depth-1, "white", white.row, white.col);//stores maxval so that debugger will go into maxval
				if(v>u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
				//printf("v = %d\n",v);
			}
			//undo(&history, walltrack, size, 1);
		}
	}
	//printf("min v = %d\n",v);
	return v;
}


int maxVal(int size, char** walltrack, int depth, char* color, int row, int col){
	//printf("max\n");
	int minpathw, minpathb = size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = -9999;
	if (strcmp(color, "white")==0) {
		if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
			int debugnum = 100*((minpathw - minpathb)+epsilon*((black.walls-white.walls)+1))/minpathb;//TODO: kill this var
			//printf("max eval w = %d\n",debugnum);
			return debugnum;
		}
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		for (int i=0;i<6;i++) {
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				historyupdate(&history, color, white.row, white.col, ' ');//hostoryupdate simulates nextmove?
				//printf("i = %d\n",i); printf("new row = %d\n", white.row); printf("new col = %d\n", white.col);//TODO: kill these
				//yourcolor
				int u = minVal(size, walltrack, depth-1, "black", black.row, black.col);
				if(v<u){
				   v=u;
				}
				//printf("v = %d\n",v);
				undo(&history, walltrack, size, 1);
			}
			//undo(&history, walltrack, size, 1);
		}
	}
	else {//black
		if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
			int debugnum = 100*((minpathb - minpathw)+epsilon*((white.walls-black.walls)+1))/minpathw;//TODO: kill this var
			//printf("max eval b = %d\n",debugnum);
			return debugnum;
		}
		getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
		for (int i=0;i<6;i++) {
			if(possiblemoves[i][0]!=-1 && strcmp(color, "black")==0){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate stores nextmove?
				//printf("i = %d\n",i); printf("new row = %d\n", black.row); printf("new col = %d\n", black.col);//TODO: kill these
				//yourcolor
				int u = minVal(size, walltrack, depth-1, "white", white.row, white.col);
				if(v<u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
				//printf("v = %d\n",v);
			}
			//undo(&history, walltrack, size, 1);
		}
	}
	//printf("max v = %d\n",v);
	return v;
}

//will be kernelcall. This function chooses chosenr, chosenc, and chosenori
void minMaxDecision(int size, char** walltrack, int depth, char* color, int*chosenr, int* chosenc, char* chosenori) {
	int bestVal = -9999;
	int bestrow;
	int bestcol;
	int bestori;
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);
		for (int i=0;i<6;i++) {
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];//simulate nextMove
				white.col=possiblemoves[i][1];//simulate nextMove
				historyupdate(&history, color, white.row, white.col, ' ');//hostoryupdate simulates nextmove?
				//printf("white new row = %d\n", white.row); printf("white new col = %d\n", white.col);
				//TODO: get white to get to goal
				int currVal = minVal(size, walltrack, depth, color, white.row, white.col);
				if(currVal > bestVal){
					bestVal = currVal;
					bestrow = white.row;
					bestcol = white.col;
					bestori = ' ';
				}
			}
		}
		//*chosenr = bestrow;
		//*chosenc = bestcol;
		//*chosenori = bestori;
		undo(&history, walltrack, size, 1);
	}
	else {
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, black.row, black.col);
			if (possiblemoves[i][0]!=-1) {
				black.row=possiblemoves[i][0];//simulate nextMove
				black.col=possiblemoves[i][1];//simulate nextMove
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate stores nextmove?
				//printf("black new row = %d\n", black.row); printf("black new col = %d\n", black.col);
				int currVal = minVal(size, walltrack, depth, color, black.row, black.col);
				if(currVal > bestVal){
					bestVal = currVal;
					bestrow = black.row;
					bestcol = black.col;
					bestori = ' ';
				}
			}
		}
		//*chosenr = bestrow;
		//*chosenc = bestcol;
	    //*chosenori = bestori;
		undo(&history, walltrack, size, 1);
	}
	*chosenr = bestrow;
	*chosenc = bestcol;
	*chosenori = bestori;
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
