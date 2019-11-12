#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
#include "globals.h"

void clr(char** walls, int size) {
	// clears board and wall placements
	int i, j;
	for (i=0;i<size;i++) {
		for (j=0;j<size;j++) {
			walls[i][j]=' ';
			white.pathing[i][j]=-1;
			black.pathing[i][j]=-1;
		}
	}
	white.walls=0;
	black.walls=0;
	printf("=\n\n");
	return;
}

void setpawns(int size) {
	// sets starting position of pawns
	white.row=0;
	white.col=size/2;
	black.row=size-1;
	black.col=size/2;
}

char** createwalls(int r) {
	int i;
	char** walls;
	if (r<3 || r>25 || r%2==0) {//if boardsize is too small or too large, throw exception
		printf("? unacceptable size\n\n");
		return NULL;
	}//else, make space for walls (board width and height * sizeof type)
	walls=malloc(r*sizeof(char*));
	white.pathing=malloc(r*sizeof(int*));
	black.pathing=malloc(r*sizeof(int*));
	if (walls==NULL || white.pathing==NULL || black.pathing==NULL) {
		printf("? unacceptable size\n\n");
	}
	for (i=0;i<r;i++) {
		white.pathing[i]=malloc(r*sizeof(int));
		black.pathing[i]=malloc(r*sizeof(int));
		walls[i]=malloc(r*sizeof(char));
		if (walls[i]==NULL || white.pathing[i]==NULL || black.pathing==NULL) {
			printf("? unacceptable size\n\n");
			break;
		}
	}
	return walls;
}

void show(char** walls, int size) {
	int i, j;
	printf("=\n ");
	//print column letters
	for (i=0;i<size;i++) {
		printf(" %c", 'A'+i);
	}
	printf("\n ");
	//print top board edge
	for (i=0;i<size;i++) {
		printf("+-");
	}
	printf("+\n");
	for (i=size-1;i>=0;i--) {
		//print left edge of board
		printf("%d|", i+1);
		//print cells
		for (j=0;j<size;j++) {
			if (white.row==i && white.col==j) {
				printf("W|");
			}
			else if (black.row==i && black.col==j) {
				printf("B|");
			}
			else {
				printf(" |");
			}
		}
		printf("%d", i+1);
		printf("\n +");
		//print horizontial ends for cells
		for (j=0;j<size;j++) {
			printf("-");
			if (walls[i][j]=='V') {
				printf("|");
			}
			else if (walls[i][j]=='H') {
				printf("-");
			}
			else {
				printf("+");
			}
		}
		printf("\n");
	}
	//print column letters in bottom side
	printf(" ");
	for (i=0;i<size;i++) {
    	printf(" %c", 'A'+i);
    }
	printf("\n  Walls\n  White:%d\n  Black:%d\n", white.walls, black.walls);
	printf("\n\n");
}
