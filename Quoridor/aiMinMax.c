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
	/*if (*minpathw==0) {// is this if logic neccesary?
		*minpathw=1;
		*minpathb=size*size;
	}
	else if (*minpathb==0) {
		*minpathb=1;
		*minpathw=size*size;
	}*/
	return;
}

//Can be tweaked
float epsilon = 1.0;

//These will be kernels NOTE: 'move' just tracks whether we are simulating white or black
int minVal(int size, char** walltrack, int depth, char* color, int row, int col){
	//printf("min\n");
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = 9999;//supposed to be inf
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
			//int debugnum = 100*(minpathw - minpathb)+epsilon*((black.walls-white.walls)+1)/minpathb;//TODO: kill this var
			//printf("min minb = %d minw = %d 10+(minb-minw) = %d\n",minpathb,minpathw,10+(minpathb-minpathw));
			//printf("whitewalls = %d blackwalls = %d white.walls-black.walls+1/(minw) = %d\n\n",white.walls,black.walls,(white.walls-black.walls+1)/(minpathw));
			int evalWhite = 0;//10+(minpathb-minpathw)+(white.walls-black.walls+1)/(minpathw);//white
			evalWhite = minpathw;//scratch eval func update: seems to work? fingers crossed
			return evalWhite;
		}
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
			if (possiblemoves[i][0]!=-1) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, white.row, white.col, ' ');//Do I even Need historyUpdate? A: YES! Don't delete!
				int u = maxVal(size, walltrack, depth-1, "black", black.row, black.col);
				//v = min(v, maxVal(gameState.generateSuccessor(agentIndex, act), agentIndex+1, depth-1))
				if(v>u){
				   v=u;
				}
				undo(&history, walltrack, size, 1);
				//printf("v = %d\n",v);
			}
			//undo(&history, walltrack, size, 1);
		}
		//clrvalidmoves();
	}
	else {
		getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
			//int debugnum = 100*(minpathb - minpathw)+epsilon*((white.walls-black.walls)+1)/(double)minpathw;//TODO: kill this var
			//printf("min minw = %d minb = %d 100*(minw-minb) = %d\n",minpathw,minpathb,100*(minpathw-minpathb));
			//printf("blackwalls = %d whitewalls = %d black.walls-white.walls+1/(minb) = %d\n\n",black.walls,white.walls,black.walls-white.walls+1/(minpathb));
			int evalBlack = 0;//10+(minpathw-minpathb)+(black.walls-white.walls+1)/(minpathb);//black
			evalBlack = minpathb;//scratch eval func
			return evalBlack;
		}
		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
			if(possiblemoves[i][0]!=-1 && strcmp(color, "black")==0){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate simulates nextmove?
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
		//clrvalidmoves();
	}
	//printf("min v = %d\n",v);
	return v;
}


int maxVal(int size, char** walltrack, int depth, char* color, int row, int col){
	//printf("max\n");
	int minpathw=size*size, minpathb=size*size;
	minpathsMinMax(size, walltrack, &minpathw, &minpathb);
	int v = -9999;
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {//TODO: concern color
			//int debugnum = 100*(minpathw - minpathb)+epsilon*((black.walls-white.walls)+1)/minpathb;//TODO: kill this var
			//printf("max minb = %d minw = %d 10+(minb-minw) = %d\n",minpathb,minpathw,10+(minpathb-minpathw));
			//printf("whitewalls = %d blackwalls = %d white.walls-black.walls+1/(minw) = %d\n\n",white.walls,black.walls,white.walls-black.walls+1/(minpathw));
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
		//clrvalidmoves();
	}
	else {//black
		getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
		if (black.row==0 || white.row==size-1 || depth==0) {//NOTE: switching black and white evals stops the code
			//int debugnum = 100*(minpathb - minpathw)+epsilon*((white.walls-black.walls)+1)/(double)minpathw;//TODO: kill this var
			//printf("max minw = %d minb = %d 10+(minw-minb) = %d\n",minpathw,minpathb,10+(minpathw-minpathb));
			//printf("blackwalls = %d whitewalls = %d black.walls-white.walls+1/(minb) = %d\n\n",black.walls,white.walls,black.walls-white.walls+1/(minpathb));
			int evalBlack = 0;//10+(minpathw-minpathb)+(black.walls-white.walls+1)/(minpathb);//black
			evalBlack = minpathb;//scratch eval func
			return evalBlack;
		}

		for (int i=0;i<6;i++) {
			getvalidmoves(size, walltrack, color, black.row, black.col);//possiblemoves updated
			if(possiblemoves[i][0]!=-1 && strcmp(color, "black")==0){
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				minpathsMinMax(size, walltrack, &minpathw, &minpathb);
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate stores nextmove?
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
		//clrvalidmoves();
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
	//NOTE: do I need to switch around the kernel calls? call min your color first? update: been messing with eval functions. slapping a -1 on there doesn't cut it
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
				printf("white move = %d,%d\n", white.row,white.col);
				//TODO: get white to get to goal
				int currVal = -1*(minVal(size, walltrack, depth, color, white.row, white.col));
				printf("currVal = %d bestVal = %d\n",currVal,bestVal);
				if(currVal > bestVal){
					bestVal = currVal;
					bestrow = white.row;
					bestcol = white.col;
					bestori = ' ';
				}
			}
		}
		undo(&history, walltrack, size, 1);
	}
	else {
		//TODO: look into evalfunction, add wallplacement code
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
				historyupdate(&history, color, black.row, black.col, ' ');//hostoryupdate stores nextmove?
				printf("black move = %d,%d\n", black.row,black.col);
				int currVal = -1*(minVal(size, walltrack, depth, color, black.row, black.col));
				printf("currVal = %d bestVal = %d\n",currVal,bestVal);
				if(currVal > bestVal){
					bestVal = currVal;
					bestrow = black.row;
					bestcol = black.col;
					bestori = ' ';
				}
			}
		}
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
