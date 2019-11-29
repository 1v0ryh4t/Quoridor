#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
#include "play.h"
#include "globals.h"

void undo (History *history, char** walltrack, int size, int times) {
	char tempcolor[6];
	History temp;
	do {
		temp=NULL;
		if (*history==NULL) {
			return;
		}
		while ((*history)->next!=NULL) {
			temp=*history;
			*history=(*history)->next;
		}
		strcpy(tempcolor, (*history)->color);
		if ((*history)->orientation!=' ') {//undo wall
			walltrack[(*history)->row][(*history)->col]=' ';
			if (strcmp(tempcolor, "white")==0) {
				white.walls++;
			}
			else {
				black.walls++;
			}
			free(*history);
			*history=temp;
			if (*history!=NULL) {
				(*history)->next=NULL;
			}
		}
		else {//undo move
			free(*history);
			*history=temp;
			if (*history==NULL) {
				if (strcmp(tempcolor, "white")==0) {
					white.row=0;
					white.col=size/2;
				}
				else {
					black.row=size-1;
					black.col=size/2;
				}
			}
			else {
				(*history)->next=NULL;
				do {
					if ((*history)->orientation==' ' && strcmp(tempcolor, (*history)->color)==0) {
						if (strcmp(tempcolor, "white")==0) {
							white.row=(*history)->row;
							white.col=(*history)->col;
						}
						else {
							black.row=(*history)->row;
							black.col=(*history)->col;
						}
						break;
					}
					if ((*history)->previous!=NULL) {
						*history=(*history)->previous;
					}
					else {//if this is 1st element and there was no matching move parent
						if (strcmp(tempcolor, "white")==0) {
							white.row=0;
							white.col=size/2;
						}
						else {
							black.row=size-1;
							black.col=size/2;
						}
						break;
					}
				}while(1);//breaks will handle exiting, no need to have clause
			}
		}
		//return to original struct pointer value
		if (*history!=NULL) {
			while ((*history)->previous!=NULL) {
				*history=(*history)->previous;
			}
		}
		times--;
	}while (times>0);
	return;
}

int isvalidwall(char** walltrack, char orientation, int r, int c, int size, char* color) {
	int i, wpathexists=0, bpathexists=0;
	if (r<=0 || r>=size || c<0 || c>=size-1) {
		return 0;
	}
	else if (walltrack[r][c]!=' ') {
		return 0;
	}
	if (strcmp(color, "white")==0 && white.walls==0) {
		return 0;
	}
	else if (strcmp(color, "black")==0 && black.walls==0) {
		return 0;
	}
	if (orientation=='H') {
		if ((c-1>=0 && walltrack[r][c-1]=='H') || (c+1<size && walltrack[r][c+1]=='H')) {
			return 0;
		}
		else {//testing with wall placed
			walltrack[r][c]='H';
			getpath(size, walltrack);
			for (i=0;i<size;i++) {
				if (white.pathing[size-1][i]!=-1) {
					wpathexists=1;
				}
				if (black.pathing[0][i]!=-1) {
					bpathexists=1;
				}
			}
			if (wpathexists && bpathexists) {
				if (strcmp(color, "white")==0) {
					white.walls--;
				}
				else {
					black.walls--;
				}
				return 1;
			}
			walltrack[r][c]=' ';
			return 0;
		}
	}
	else {
		if ((r-1>=0 && walltrack[r-1][c]=='V') || (r+1<size && walltrack[r+1][c]=='V')) {
			return 0;
		}
		else {//testing with wall placed
			walltrack[r][c]='V';
			getpath(size, walltrack);
			for (i=0;i<size;i++) {
				if (white.pathing[size-1][i]!=-1) {
					wpathexists=1;
				}
				if (black.pathing[0][i]!=-1) {
					bpathexists=1;
				}
			}
			if (wpathexists && bpathexists) {
				if (strcmp(color, "white")==0) {
					white.walls--;
				}
				else {
					black.walls--;
				}
				return 1;
			}
			walltrack[r][c]=' ';
			return 0;
		}
	}
}

//function to get path to end

void getpath (int size, char** walltrack) {
	int i, j, moves=0, k;
	white.minpath=size*size;
	black.minpath=size*size;
	for (i=0;i<size;i++) { //reset paths
		for (j=0;j<size;j++) {
			white.pathing[i][j]=-1;
			black.pathing[i][j]=-1;
		}
	}
	white.pathing[white.row][white.col]=moves;
	if (white.row==size-1) {
		white.minpath=0;
	}
	black.pathing[black.row][black.col]=moves;
	if (black.row==0) {
		black.minpath=0;
	}
	int temp;
	do {
		temp=0;
		for (k=0;k<size;k++) {
			for (j=0;j<size;j++) {
				if (white.pathing[k][j]==moves) {
					getvalidmoves(size, walltrack, "white", k, j);
					for (i=0;i<6;i++) {
						if (possiblemoves[i][0]!=-1 && white.pathing[possiblemoves[i][0]][possiblemoves[i][1]]==-1) {
							white.pathing[possiblemoves[i][0]][possiblemoves[i][1]]=moves+1;
							temp=1;
							if (possiblemoves[i][0]==size-1 && white.pathing[possiblemoves[i][0]][possiblemoves[i][1]]<white.minpath) {
								white.minpath=white.pathing[possiblemoves[i][0]][possiblemoves[i][1]];
							}
						}
					}
				}
				if (black.pathing[k][j]==moves) {
					getvalidmoves(size, walltrack, "black", k, j);
					for (i=0;i<6;i++) {
						if (possiblemoves[i][0]!=-1 && black.pathing[possiblemoves[i][0]][possiblemoves[i][1]]==-1) {
							black.pathing[possiblemoves[i][0]][possiblemoves[i][1]]=moves+1;
							temp=1;
							if (possiblemoves[i][0]==0 && black.pathing[possiblemoves[i][0]][possiblemoves[i][1]]<black.minpath) {
								black.minpath=black.pathing[possiblemoves[i][0]][possiblemoves[i][1]];
							}
						}
					}
				}
			}
		}
		moves++;
		//stop when no more possible moves for both colors
	}while(temp);
	return;
}

void historyupdate (History *history, char* color, int r, int c, char orientation)  {
	History temp=NULL;
	while(*history!=NULL) {
		temp=(*history);
		history=&((*history)->next);
	}
	*history=malloc(sizeof(struct hlist));
	strcpy((*history)->color, color);
	(*history)->previous=temp;
	(*history)->row=r;
	(*history)->col=c;
	(*history)->orientation=orientation;
	(*history)->next=NULL;
	return;
}

void clrhistory(History *history) {
	History temp=NULL;
	if (*history==NULL) {
		return;
	}
	while ((*history)->next!=NULL) {
		temp=*history;
		*history=(*history)->next;
	}
	if (temp!=NULL) {
		do {
			free(*history);
			*history=temp;
			(*history)->next=NULL;
			if ((*history)->previous!=NULL) {
				temp=(*history)->previous;
			}
		}while ((*history)->previous!=NULL);
	}
	free(*history);
	*history=NULL;
	return;
}

void printhistory (History *history) {
	while ((*history)!=NULL) {
		printf("%s ", (*history)->color);
		printf("%c%d ", ((*history)->col)+'A', ((*history)->row)+1);
		printf("%c\n", (*history)->orientation);
		history=&((*history)->next);
	}
}

int isvalidmove(char** walltrack, int size, int r, int c, char* color) {
	/* checks if given move is valid
		returns 0 on invalid move or syntax error, 1 on valid move
	*/
	int i;
	if (!(r<0 || c<0 || c>size-1 || r>size-1)) {
		//getvalidmoves(size, walltrack, color);
	}
	else {
		return 0;
	}
	if (strcmp(color, "white")==0) {
		getvalidmoves(size, walltrack, color, white.row, white.col);
		for (i=0;i<6;i++) {
			if (r==possiblemoves[i][0] && c==possiblemoves[i][1] && r!=-1 && c!=-1) {
				printf("=\n\n");
				return 1;
			}
		}
		return 0;
	}
	else if (strcmp(color, "black")==0) {
		getvalidmoves(size, walltrack, color, black.row, black.col);
		for (i=0;i<6;i++) {
			if (r==possiblemoves[i][0] && c==possiblemoves[i][1] && r!=-1 && c!=-1) {
				printf("=\n\n");
				return 1;
			}
		}
		return 0;
	}
	return 0;
}

void updatepawn(char* color, int r, int c) {
	if (strcmp(color, "white")==0) {
		white.row=r;
		white.col=c;
	}
	else if (strcmp(color, "black")==0) {
		black.row=r;
		black.col=c;
	}
	return;
}
//resets possiblemoves
void clrvalidmoves(){
	for (int i=0;i<6;i++) {
		for (int j=0;j<2;j++) {
			possiblemoves[i][j]=-1;
		}
	}
}

void getvalidmoves(int size, char** walltrack, char* color, int row, int col) {
	int i, j;
	//for white
	if (strcmp(color, "white")==0) {
		for (i=0;i<6;i++) {
			for (j=0;j<2;j++) {
				possiblemoves[i][j]=-1;
			}
		}
		if (row+1<size && !(col-1>=0 && walltrack[row+1][col-1]=='H') &&  walltrack[row+1][col]!='H') //for up move
		{
			if (black.row!=row+1 || black.col!=col) {//normal move
				possiblemoves[0][0]=row+1;
				possiblemoves[0][1]=col;
			}
			else if (row+2<size && !(col-1>=0 && walltrack[row+2][col-1]=='H') && walltrack[row+2][col]!='H') { //jump
				possiblemoves[4][0]=row+2;
				possiblemoves[4][1]=col;
			}
			else {
				if (col-1>=0 && !(row+2<size && walltrack[row+2][col-1]=='V') && walltrack[row+1][col-1]!='V') {//jump left
					possiblemoves[4][0]=row+1;
					possiblemoves[4][1]=col-1;
				}
				if (!(row+2<size && walltrack[row+2][col]=='V') && col+1<size && walltrack[row+1][col]!='V') {//jump right
					possiblemoves[5][0]=row+1;
					possiblemoves[5][1]=col+1;
				}
			}
		}
		if (walltrack[row][col]!='H' && row-1>=0 && !(col-1>=0 && walltrack[row][col-1]=='H')) //for down move
                {
			if (black.row!=row-1 || black.col!=col) {//normal move
                        	possiblemoves[1][0]=row-1;
                        	possiblemoves[1][1]=col;
			}
			else if (row-2>=0 && !(col-1>=0 && walltrack[row-1][col-1]=='H') && walltrack[row-1][col]!='H') {//jump
					possiblemoves[4][0]=row-2;
					possiblemoves[4][1]=col;
			}
			else {
				if (col-1>=0 && walltrack[row-1][col-1]!='V' && walltrack[row][col-1]!='V') {//jump right
					possiblemoves[4][0]=row-1;
					possiblemoves[4][1]=col-1;
				}
				if (col+1<size && walltrack[row][col]!='V' && walltrack[row-1][col]!='V') { //jump left
					possiblemoves[5][0]=row-1;
					possiblemoves[5][1]=col+1;
				}
			}
                }
		if (col-1>=0 && walltrack[row][col-1]!='V' && !(row+1<size && walltrack[row+1][col-1]=='V')) {//for left move
			if (black.row!=row || black.col!=col-1) {
				possiblemoves[2][0]=row;
				possiblemoves[2][1]=col-1;
			}
			else if (col-2>=0 && walltrack[row][col-2]!='V' && !(row+1<size && walltrack[row+1][col-2]=='V')) {//jump
				possiblemoves[4][0]=row;
				possiblemoves[4][1]=col-2;
			}
			else {
				if (row-1>0 && !(col-2>=0 && walltrack[row][col-2]=='H') && walltrack[row][col-1]!='H') {//jump left
                    possiblemoves[4][0]=row-1;
                	possiblemoves[4][1]=col-1;
                }
				if (row+1<size && !(col-2>=0 && walltrack[row+1][col-2]=='H') && walltrack[row+1][col-1]!='H') { //jump right
					possiblemoves[5][0]=row+1;
					possiblemoves[5][1]=col-1;
				}
			}
		}
		if (col+1<size && walltrack[row][col]!='V' && !(row+1<size && walltrack[row+1][col]=='V')) {//for right move
			if (black.row!=row || black.col!=col+1) {
				possiblemoves[3][0]=row;
				possiblemoves[3][1]=col+1;
			}
			else if (col+2<size && walltrack[row][col+1]!='V' && !(row+1<size && walltrack[row+1][col+1]=='V')) {//jump
				possiblemoves[4][0]=row;
				possiblemoves[4][1]=col+2;
			}
			else {
				if (row+1<size && walltrack[row+1][col]!='H' && walltrack[row+1][col+1]!='H') { //jump left
					possiblemoves[4][0]=row+1;
					possiblemoves[4][1]=col+1;
				}
				if (row-1>=0 && walltrack[row][col]!='H' && walltrack[row][col+1]!='H') { //jump right
					possiblemoves[5][0]=row-1;
					possiblemoves[5][1]=col+1;
				}
			}
		}
	}
	//for black
	else if (strcmp(color, "black")==0) {
                for (i=0;i<6;i++) {
                        for (j=0;j<2;j++) {
                                possiblemoves[i][j]=-1;
                        }
                }
                if (row+1<size && !(col-1>=0 && walltrack[row+1][col-1]=='H') &&  walltrack[row+1][col]!='H') //for up move
                {
                        if (white.row!=row+1 || white.col!=col) {//normal move
                                possiblemoves[1][0]=row+1;//changes so down is default for black
                                possiblemoves[1][1]=col;//changes so down is default for black
                        }
                        else if (row+2<size && !(col-1>=0 && walltrack[row+2][col-1]=='H') && walltrack[row+2][col]!='H') { //jump
                                possiblemoves[4][0]=row+2;
                                possiblemoves[4][1]=col;
                        }
                        else {
                                if (col-1>=0 && !(row+2<size && walltrack[row+2][col-1]=='V') && walltrack[row+1][col-1]!='V') {//jump left
                                        possiblemoves[4][0]=row+1;
                                        possiblemoves[4][1]=col-1;
                                }
                                if (!(row+2<size && walltrack[row+2][col]=='V') && col+1<size && walltrack[row+1][col]!='V') {//jump right
                                        possiblemoves[5][0]=row+1;
                                        possiblemoves[5][1]=col+1;
                                }
                        }
                }
                if (walltrack[row][col]!='H' && row-1>=0 && !(col-1>=0 && walltrack[row][col-1]=='H')) //for down move
                {
                        if (white.row!=row-1 || white.col!=col) {//normal move
                                possiblemoves[0][0]=row-1;//changes so down is default for black
                                possiblemoves[0][1]=col;//changes so down is default for black
                        }
                        else if (row-2>=0 && !(col-1>=0 && walltrack[row-1][col-1]=='H') && walltrack[row-1][col]!='H') {//jump
                                        possiblemoves[4][0]=row-2;
                                        possiblemoves[4][1]=col;
                        }
                        else {
                                if (col-1>=0 && walltrack[row-1][col-1]!='V' && walltrack[row][col-1]!='V') {//jump right
                                        possiblemoves[4][0]=row-1;
                                        possiblemoves[4][1]=col-1;
                                }
                                if (col+1<size && walltrack[row][col]!='V' && walltrack[row-1][col]!='V') { //jump left
                                        possiblemoves[5][0]=row-1;
                                        possiblemoves[5][1]=col+1;
                                }
                        }
                }
		if (col-1>=0 && walltrack[row][col-1]!='V' && !(row+1<size && walltrack[row+1][col-1]=='V')) {//for left move
                        if (white.row!=row || white.col!=col-1) {
                                possiblemoves[2][0]=row;
                                possiblemoves[2][1]=col-1;
                        }
                        else if (col-2>=0 && walltrack[row][col-2]!='V' && !(row+1<size && walltrack[row+1][col-2]=='V')) {//jump
                                possiblemoves[4][0]=row;
                                possiblemoves[4][1]=col-2;
                        }
                        else {
                                if (row-1>0 && !(col-2>=0 && walltrack[row][col-2]=='H') && walltrack[row][col-1]!='H') {//jump left
                                        possiblemoves[4][0]=row-1;
                                        possiblemoves[4][1]=col-1;
                                }
                                if (row+1<size && !(col-2>=0 && walltrack[row+1][col-2]=='H') && walltrack[row+1][col-1]!='H') { //jump right
                                        possiblemoves[5][0]=row+1;
                                        possiblemoves[5][1]=col-1;
                                }
                        }
                }
		if (col+1<size && walltrack[row][col]!='V' && !(row+1<size && walltrack[row+1][col]=='V')) {//for right move
                        if (white.row!=row || white.col!=col+1) {
                                possiblemoves[3][0]=row;
                                possiblemoves[3][1]=col+1;
                        }
                        else if (col+2<size && walltrack[row][col+1]!='V' && !(row+1<size && walltrack[row+1][col+1]=='V')) {//jump
                                possiblemoves[4][0]=row;
                                possiblemoves[4][1]=col+2;
                        }
                        else {
                                if (row+1<size && walltrack[row+1][col]!='H' && walltrack[row+1][col+1]!='H') { //jump left
                                        possiblemoves[4][0]=row+1;
                                        possiblemoves[4][1]=col+1;
                                }
                                if (row-1>=0 && walltrack[row][col]!='H' && walltrack[row][col+1]!='H') { //jump right
                                        possiblemoves[5][0]=row-1;
                                        possiblemoves[5][1]=col+1;
                                }
                        }
                }
	}
	return;
}
