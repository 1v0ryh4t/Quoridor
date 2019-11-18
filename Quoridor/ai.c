#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include <string.h>
#include "play.h"
#include "globals.h"
#include "ai.h"

//is this neccesary??
void minpaths(int size, char **walltrack, int *minpathw, int *minpathb) {
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

//will change this to minimax
void getmove(int size, char** walltrack, char* color, int move, int *wins, int *sims, int *chosenr, int *chosenc, char *chosenori, int analdepth, int* foundmove, double startvaluew, double startvalueb) {
	int i, j, minpathw=size*size, minpathb=size*size;
	double value, highvalue=-10.0, initvalue;
	minpaths(size, walltrack, &minpathw, &minpathb);
	//startvalues are eval funcs. can condense down to AI playing on black only. start at 0.5
	startvaluew=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
	startvalueb=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
	if (move==2) {//check for winning move immidiately Move starts at 1
		if (strcmp(color, "white")==0) {//if black at whiterow
			if (black.row==0) {
				*wins=1;
				*sims=1;
				return;
			}
		}
		else {//if white in blackrow
			if (white.row==size-1) {
				*wins=1;
				*sims=1;
				return;
			}
		}
	}
	if (move==3) {//check for opponent winning move immediately
		if (strcmp(color, "white")==0) {
			if (black.row==0) {
				*wins=0;
				*sims=1000;
				return;
			}
		}
		else {
			if (white.row==size-1) {
				*wins=0;
				*sims=1000;
				return;
			}
		}
	}
	if (move>analdepth) {
		minpaths(size, walltrack, &minpathw, &minpathb);
		if (strcmp(color, "white")==0) {//we are on an odd move, so same color as the one we want to play
			if (minpathw<=minpathb) {
				(*wins)++;
			}
		}
		else {
			if (minpathb<=minpathw) {
				(*wins)++;
			}
		}
		(*sims)++;
		return;
	}
	for (i=0;i<6;i++) {
		if (strcmp(color, "white")==0) {
			getvalidmoves(size, walltrack, color, white.row, white.col);
		}
		else {
			getvalidmoves(size, walltrack, color, black.row, black.col);
		}
		if (possiblemoves[i][0]!=-1) {
			if (strcmp(color, "white")==0) {
				white.row=possiblemoves[i][0];
				white.col=possiblemoves[i][1];
				minpaths(size, walltrack, &minpathw, &minpathb);
				initvalue=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
				historyupdate(&history, color, white.row, white.col, ' ');
				if (initvalue<startvaluew) {
					undo(&history, walltrack, size, 1);
					continue;
				}
				initvalue=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
				if (initvalue<startvalueb) {
					if (move%2==0) {
						undo(&history, walltrack, size, 1);
						return;
					}
				}
				getmove(size, walltrack, "black", move+1, wins, sims, chosenr, chosenc, chosenori, analdepth, foundmove, startvaluew, startvalueb);
				if (move==1) {
					minpaths(size, walltrack, &minpathw, &minpathb);
					if (*sims==0) {
						*sims=1;
					}
					value=2*((double)(*wins))/((double)(*sims))+((double)minpathb)/((double)minpathw);
					if (value>highvalue) {
						*chosenr=white.row;
						*chosenc=white.col;
						*chosenori=' ';
						if (value>=0.01) {
							*foundmove=1;
						}
						highvalue=value;
					}
					*sims=0;
					*wins=0;
				}
				undo(&history, walltrack, size, 1);
			}
			else {
				black.row=possiblemoves[i][0];
				black.col=possiblemoves[i][1];
				minpaths(size, walltrack, &minpathw, &minpathb);
				initvalue=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
				historyupdate(&history, color, black.row, black.col, ' ');
				if (initvalue<startvalueb) {
					if (move==1) {
					}
					undo(&history, walltrack, size, 1);
					continue;
				}
				initvalue=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
				if (initvalue<startvaluew) {
					if (move%2==0) {
						undo(&history, walltrack, size, 1);
						return;
					}
				}
				getmove(size, walltrack, "white", move+1, wins, sims, chosenr, chosenc, chosenori, analdepth, foundmove, startvaluew, startvalueb);
				if (move==1) {
					minpaths(size, walltrack, &minpathw, &minpathb);
					if (*sims==0) {
						*sims=1;
					}
					value=2*((double)(*wins))/((double)(*sims))+((double)minpathw)/((double)minpathb);
					if (value>highvalue) {
						*chosenr=black.row;
						*chosenc=black.col;
						*chosenori=' ';
						if (value>=0.01) {
							*foundmove=1;
						}
						highvalue=value;
					}
					*sims=0;
					*wins=0;
				}
				undo(&history, walltrack, size, 1);
			}
		}
	}
	if (strcmp(color, "white")==0) {
		for (i=1;i<=size;i++) {
			for (j=0;j<=size-1;j++) {
				minpaths(size, walltrack, &minpathw, &minpathb);
				if (black.pathing[i-1][j]>5) {
					continue;
				}
				if (isvalidwall(walltrack, 'H', i, j, size, "white")) {
					walltrack[i][j]='H';
					historyupdate(&history, color, i, j, 'H');
					minpaths(size, walltrack, &minpathw, &minpathb);
					initvalue=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
					if (initvalue<startvaluew) {
						undo(&history, walltrack, size, 1);
						continue;
					}
					initvalue=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
					if (initvalue<startvalueb) {
						if (move%2==0) {
							undo(&history, walltrack, size, 1);
							return;
						}
					}
					getmove(size, walltrack, "black", move+1, wins, sims, chosenr, chosenc, chosenori, analdepth, foundmove, startvaluew, startvalueb);
					if (move==1) {
						minpaths(size, walltrack, &minpathw, &minpathb);
						if (*sims==0) {
							*sims=1;
						}
						value=2*((double)(*wins))/((double)(*sims))+((double)minpathb)/((double)minpathw);
						if (value>highvalue) {
							*chosenr=i;
							*chosenc=j;
							*chosenori='H';
							if (value>=0.01) {
								*foundmove=1;
							}
							highvalue=value;
						}
						*sims=0;
						*wins=0;
					}
					undo(&history, walltrack, size, 1);
				}
			}
		}
		for (i=1;i<=size;i++) {
			for (j=0;j<=size-1;j++) {
				minpaths(size, walltrack, &minpathw, &minpathb);
				if (black.pathing[i-1][j]>5) {
					continue;
				}
				if (isvalidwall(walltrack, 'V', i, j, size, "white")) {
					walltrack[i][j]='V';
					historyupdate(&history, color, i, j, 'V');
					minpaths(size, walltrack, &minpathw, &minpathb);
					initvalue=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
					if (initvalue<startvaluew) {
						undo(&history, walltrack, size, 1);
						continue;
					}
					initvalue=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
					if (initvalue<startvalueb) {
						if (move%2==0) {
							undo(&history, walltrack, size, 1);
							return;
						}
					}
					getmove(size, walltrack, "black", move+1, wins, sims, chosenr, chosenc, chosenori, analdepth, foundmove, startvaluew, startvalueb);
					if (move==1) {
						minpaths(size, walltrack, &minpathw, &minpathb);
						if (*sims==0) {
							*sims=1;
						}
						value=2*((double)(*wins))/((double)(*sims))+((double)minpathb)/((double)minpathw);
						if (value>highvalue) {
							*chosenr=i;
							*chosenc=j;
							*chosenori='V';
							if (value>=0.01) {
								*foundmove=1;
							}
							highvalue=value;
						}
						*sims=0;
						*wins=0;
					}
					undo(&history, walltrack, size, 1);
				}
			}
		}
	}
	else {
		for (i=1;i<=size;i++) {
			for (j=0;j<=size-1;j++) {
				minpaths(size, walltrack, &minpathw, &minpathb);
				if (white.pathing[i-1][j]>5) {
					continue;
				}
				if (isvalidwall(walltrack, 'H', i, j, size, "black")) {
					walltrack[i][j]='H';
					historyupdate(&history, color, i, j, 'H');
					minpaths(size, walltrack, &minpathw, &minpathb);
					initvalue=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
					if (initvalue<startvalueb) {
						undo(&history, walltrack, size, 1);
						continue;
					}
					initvalue=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
					if (initvalue<startvaluew) {
						if (move%2==0) {
							undo(&history, walltrack, size, 1);
							return;
						}
					}
					getmove(size, walltrack, "white", move+1, wins, sims, chosenr, chosenc, chosenori, analdepth, foundmove, startvaluew, startvalueb);
					if (move==1) {
						minpaths(size, walltrack, &minpathw, &minpathb);
						if (*sims==0) {
							*sims=1;
						}
						value=2*((double)(*wins))/((double)(*sims))+((double)minpathw)/((double)minpathb);
						if (value>highvalue) {
							*chosenr=i;
							*chosenc=j;
							*chosenori='H';
							if (value>=0.01) {
								*foundmove=1;
							}
							highvalue=value;
						}
						*sims=0;
						*wins=0;
					}
					undo(&history, walltrack, size, 1);
				}
			}
		}
		for (i=1;i<=size;i++) {
			for (j=0;j<=size-1;j++) {
				minpaths(size, walltrack, &minpathw, &minpathb);
				if (white.pathing[i-1][j]>5) {
					continue;
				}
				if (isvalidwall(walltrack, 'V', i, j, size, "black")) {
					walltrack[i][j]='V';
					historyupdate(&history, color, i, j, 'V');
					minpaths(size, walltrack, &minpathw, &minpathb);
					initvalue=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
					if (initvalue<startvalueb) {
						undo(&history, walltrack, size, 1);
						continue;
					}
					initvalue=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
					if (initvalue<startvaluew) {
						if (move%2==0) {
							undo(&history, walltrack, size, 1);
							return;
						}
					}
					getmove(size, walltrack, "white", move+1, wins, sims, chosenr, chosenc, chosenori, analdepth, foundmove, startvaluew, startvalueb);
					if (move==1) {
						minpaths(size, walltrack, &minpathw, &minpathb);
						if (*sims==0) {
							*sims=1;
						}
						value=2*((double)(*wins))/((double)(*sims))+((double)minpathw)/((double)minpathb);
						if (value>highvalue) {
							*chosenr=i;
							*chosenc=j;
							*chosenori='V';
							if (value>=0.01) {
								*foundmove=1;
							}
							highvalue=value;
						}
						*sims=0;
						*wins=0;
					}
					undo(&history, walltrack, size, 1);
				}
			}
		}
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
