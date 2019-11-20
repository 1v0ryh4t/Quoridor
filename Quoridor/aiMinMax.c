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
	int minpathw, minpathb = size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
		int debugnum = ((minpathw - minpathb)+epsilon*(white.walls-black.walls));//TODO: kill this var
		return debugnum;
	}
	int v = 9999;//supposed to be inf
	//iterate through all legal actions using possiblemoves
	if (strcmp(color, "white")==0) {
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				//minpathsMinMax(size, walltrack, &minpathw, &minpathb);//call this for some reason?
				//historyupdate(&history, color, white.row, white.col, ' ');//Do I even Need historyUpdate?
				//printf("i = %d\n",i); printf("history row = %d\n", history->row); printf("history col = %d\n", history->col);//TODO: kill these
				printf("i = %d\n",i); printf("new row = %d\n", white.row); printf("new col = %d\n", white.col);//TODO: kill these
				if (strcmp(color, "white")==0) {//TODO: delete this statement
					//mycolor
					//NOTE: does myColor always launch max and yourColor always launch min?? is agent decrement useless because we are only tracking 2 agents?
					//v = min(v, minVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth))
					//TODO: should I be calling black nextstate here? to simulate incrementing agent? IDK
					int u = minVal(size, walltrack, depth, "black", black.row, black.col);
					if(v>u){
					   v=u;//this is total nextstate because we have newPos and running walltrack
					}
				}
			}
			//undo(&history, walltrack, size, 1);
		}
	}
	else {
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
			if(possiblemoves[i][0]!=-1 && strcmp(color, "black")==0){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate simulates nextmove?
				printf("i = %d\n",i); printf("history row = %d\n", history->row); printf("history col = %d\n", history->col);//TODO: kill these
				if (strcmp(color, "black")==0) {//TODO: delete this statement
					//yourcolor
					//v = min(v, minVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth))
					int u = maxVal(size, walltrack, depth-1, color, black.row, black.col);//stores maxval so that debugger will go into maxval
					if(v<u){
					   v=u;//this is total nextstate because we have newPos and running walltrack
					}
				}
			}
			undo(&history, walltrack, size, 1);
		}
	}
	return v;
}


int maxVal(int size, char** walltrack, int depth, char* color, int row, int col){
	int minpathw, minpathb = size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
		int debugnum = ((minpathw - minpathb)+epsilon*(white.walls-black.walls));//TODO: kill this var
		return debugnum;
	}
	int v = -9999;
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		for (int i=0;i<6;i++) {
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				historyupdate(&history, color, white.row, white.col, ' ');//hostoryupdate stores nextmove?
				printf("i = %d\n",i);//TODO: kill these
				printf("history row = %d\n", history->row);//TODO: kill these
				printf("history col = %d\n", history->col);//TODO: kill these
				//TODO: does this mean that I am projecting mycolor?
				//for each possible action (possiblemoves[i][j] != -1),
                //v = max(v, minVal(gameState.generateSuccessor(0, act),1,depth))

			}
		}
	}
	else {
		getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
		for (int i=0;i<6;i++) {
			if(possiblemoves[i][0]!=-1 && strcmp(color, "black")==0){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate stores nextmove?
				printf("i = %d\n",i);//TODO: kill these
				printf("history row = %d\n", history->row);//TODO: kill these
				printf("history col = %d\n", history->col);//TODO: kill these
				//for each possible action (possiblemoves[i][j] != -1),
				//if my color,  v = min(v, minVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth))
				//if yourcolor, v = min(v, maxVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth-1))
			}
		}
	}
	return v;
}

//will be kernelcall. This function chooses chosenr, chosenc, and chosenori
void minMaxDecision(int size, char** walltrack, int depth, char* color, int*chosenr, int* chosenc, char* chosenori) {
/*#returns the Alpha in Actions(state) maximizing the min(result(alpha,state))
	self.bestMove = Directions.EAST
	bestVal = float('-inf')
	for act in gameState.getLegalActions(0):
		succState = gameState.generateSuccessor(0, act)

		currentValue = minVal(succState,1, self.depth)
		if(currentValue > bestVal):
			bestVal = currentValue
			self.bestMove = act

	return self.bestMove*/
	if(strcmp(color, "white")==0){
		minVal(size, walltrack, depth, color, white.row, white.col);
	}
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
