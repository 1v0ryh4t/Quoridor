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

//These will be kernels NOTE: 'move' just tracks whether we are simulating white or black
int minVal(int size, char** walltrack, char* color, int move, int *chosenr, int *chosenc, char *chosenori, int lookdepth, int* foundmove, double startvaluew, double startvalueb){
	/* if terminal test(state) then return utility(state) determined by black's path and whites minpath and also win/loss rato apparently?
	 * eval function should be ((whitepath-blackpath)+epsilon*(numwhitewalls-numblackwalls))
	 * else, set v = inf
	 * go through all legalactions
	 * get white min(v, maxVal(nextstate))
	 *
	 */
	int v = 0;
	return v;//TODO: make a variable
}


int maxVal(int size, char** walltrack, char* color, int move, int *chosenr, int *chosenc, char *chosenori, int lookdepth, int* foundmove, double startvaluew, double startvalueb){

	int v = 0;
	return v;//TODO: make a variable
}

//this maybe shouldn't be void???? it returns the next state, but what do I track????
//A: The utility, the value of the next state.
//How??? keep a tally, evaluate the path from goal
int nextState(){//nextstate uses chosenrow and chosencol to updatepawn

	return 0;//TODO: should be tally
}

//will change this to minimaxDecision, will be kernelcall
void minMaxDecision(int size, char** walltrack, char* color, int move, int* wins, int* sims, int*chosenr, int* chosenc, char* chosenori, int lookdepth, int* foundmove, double startvaluew, double startvalueb) {
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
                    nextState = gameState.generateSuccessor(agentIndex, act)
                    v = min(v, minVal(nextState, agentIndex+1, depth))
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
