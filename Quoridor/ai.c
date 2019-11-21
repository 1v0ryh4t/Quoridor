#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include <string.h>
#include "play.h"
#include "globals.h"
#include "ai.h"

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

void getmove(int size, char** walltrack, char* color, int move, int *wins, int *sims, int *chosenr, int *chosenc, char *chosenori, int analdepth, int* foundmove, double startvaluew, double startvalueb) {
	int i, j, minpathw=size*size, minpathb=size*size;
	double value, highvalue=-10.0, initvalue;
	minpaths(size, walltrack, &minpathw, &minpathb);
	startvaluew=100*(minpathb-minpathw)+white.walls-black.walls+1/((double)minpathw);
	startvalueb=100*(minpathw-minpathb)+black.walls-white.walls+1/((double)minpathb);
	if (move==2) {//check for winning move immidiately
		if (strcmp(color, "white")==0) {
			if (black.row==0) {//if black wins
				*wins=1;
				*sims=1;
				return;
			}
		}
		else {//if color black
			if (white.row==size-1) {//if white wins
				*wins=1;
				*sims=1;
				return;
			}
		}
	}
	if (move==3) {//check for opponent winning move immidiately
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
				historyupdate(&history, color, white.row, white.col, ' ');//hostoryupdate stores nextmove?
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
