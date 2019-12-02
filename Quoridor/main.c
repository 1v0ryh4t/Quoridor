#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
#include "play.h"
#include "ai.h"
#include "aiMinMaxCUDA.h"
#include "globals.h"

void inputformat(char* fullarg) {
	while ((*fullarg)!='\0') {
		if ((*fullarg)<32 || (*fullarg)==127) {
			if ((*fullarg)!=9 && (*fullarg)!=10) {
				(*fullarg)=' ';
			}
		}
		fullarg++;
	}
	return;
}

int main(void) {
	void minMaxDecision(int size, char** walltrack, int depth, char* color, int*chosenr, int* chosenc, char* chosenori);
	char fullarg[30], *arg, name[]="ABPro", **walltrack=NULL, col, *colortemp, orientation[12], *orientationcp, selori=' ', color[6], chosenori='t';
	int i, size, r, c, chosenrow=-1, chosencol=-1, wins=0, sims=0, foundmove, treedepth /*he he*/;
	history=NULL;
	double startvaluew, startvalueb;
	char ch, commands[13][20]={"name", "known_command", "list_commands",
			"quit", "boardsize", "clear_board", "walls",
			"playmove", "playwall", "genmove", "undo",
			"winner", "showboard"};
	do {
		do {
			fgets(fullarg, sizeof(fullarg), stdin);
			inputformat(fullarg);
			strtok(fullarg, "#"); //removing # and following characters
			arg=strtok(fullarg, " \n\t");
		} while (arg==NULL);
		//ADMIN COMMANDS
		if (strcmp(arg, "name")==0) {
			printf("= %s\n\n", name);
		}
		else if (strcmp(arg, "quit")==0) {
			printf("=\n\n");
			return 0;
		}
		else if (strcmp(arg, "list_commands")==0) {
			printf("=");
			for (i=0;i<13;i++) {
				printf("%s\n", commands[i]);
			}
			printf("\n\n");
		}
		else if (strcmp(arg, "known_command")==0) {
			scanf("%14s", arg);
			for (i=0;i<13;i++) {
				if (strcmp(arg, commands[i])==0) {
					printf("= true\n\n");
					i=-1;
					break;
				}
			}
			if (i!=-1) {
				printf("= false\n\n");
			}
		}
		//SETUP COMMANDS
		else if (strcmp(arg, "boardsize")==0) {
            arg=strtok(NULL, " \n\t");
            if (arg!=NULL) {
            	if (walltrack!=NULL) {
            		for (i=0;i<size;i++) {
            			free(walltrack[i]);
            		}
            		free(walltrack);
            	}
           		size=atoi(arg);
				walltrack=createwalls(size);
				if (walltrack!=NULL) {
        			clr(walltrack, size);
        			clrhistory(&history);
					setpawns(size);
				}
			}
			else {
				printf("? syntax error\n\n");
				continue;
			}
		}
        else if (strcmp(arg, "clear_board")==0) {
            clr(walltrack, size);
            clrhistory(&history);
			setpawns(size);
       	 }
        else if (strcmp(arg, "walls")==0) {
            arg=strtok(NULL, " \n\t");
            if (arg!=NULL) {
            	white.walls=atoi(arg);
				black.walls=white.walls;
        		printf("=\n\n");
        	}
		}
        else if (strcmp(arg, "showboard")==0) {
			if (walltrack==NULL) {
				printf("? empty board\n\n");
				continue;
			}
			show(walltrack, size);
        }
		//PLAY COMMANDS
		else if (strcmp(arg, "playmove")==0) {
			colortemp=strtok(NULL, " \n\t");
			if (colortemp!=NULL) {
				strcpy(color, colortemp);
				if (strcmp(color, "WHITE")==0 || strcmp(color, "w")==0 || strcmp(color, "W")==0) {
					strcpy(color, "white");
				}
				else if (strcmp(color, "BLACK")==0 || strcmp(color, "b")==0 || strcmp(color, "B")==0) {
					strcpy(color, "black");
				}
       			if (strcmp(color, "white") && strcmp(color, "black")) {
               		printf("? syntax error\n\n");
       				continue;
				}
				else {
        			arg=strtok(NULL, " \n\t");
        			if (arg==NULL) {
        				printf("? syntax error\n\n");
        				continue;
        			}
        			col=(*arg++); //1st element is column
        			r=atoi(arg);
        			if (col>='a') {
               				c=col-'a';
        			}
        			else {
               				c=col-'A';
        			}
					r--;
					if (isvalidmove(walltrack, size, r, c, color)) {
						orientation[0]=' ';
						updatepawn(color, r, c);
						historyupdate(&history, color, r, c, orientation[0]);
					}
					else {
						printf("? illegal move\n\n");
					}
				}
			}
			else {
				printf("? syntax error\n\n");
				continue;
			}
		}
		else if (strcmp(arg, "playwall")==0) {
			colortemp=strtok(NULL, " \n\t");
			if (colortemp==NULL) {
				printf("? syntax error\n\n");
				continue;
			}
			strcpy(color, colortemp);
			if (strcmp(color, "WHITE")==0 || strcmp(color, "w")==0 || strcmp(color, "W")==0) {
				strcpy(color, "white");
			}
			else if (strcmp(color, "BLACK")==0 || strcmp(color, "b")==0 || strcmp(color, "B")==0) {
				strcpy(color, "black");
			}
			if (strcmp(color, "white") && strcmp(color, "black")) {
				printf("? syntax error\n\n");
				continue;
			}
			else {
				arg=strtok(NULL, " \n\t");
				if (arg==NULL) {
					printf("? syntax error\n\n");
					continue;
				}
				col=(*arg++);
				r=atoi(arg);
				if (col>='a') {
					c=col-'a';
				}
				else {
					c=col-'A';
				}
				r--;
				orientationcp=strtok(NULL, " \n\t");
				if (orientationcp==NULL) {
					printf("? syntax error\n\n");
					continue;
				}
				strcpy(orientation, orientationcp);
				if (strcmp(orientation, "HORIZONTAL")==0 || strcmp(orientation, "horizontal")==0 || strcmp(orientation, "h")==0 || strcmp(orientation, "H")==0) {
					orientation[0]='H';
				}
				else if (strcmp(orientation, "VERTICAL")==0 || strcmp(orientation, "vertical")==0 || strcmp(orientation, "v")==0 || strcmp(orientation, "V")==0) {
					orientation[0]='V';
				}
				else {
					printf("? syntax error\n\n");
					continue;
				}
				//find possible walls, check for validity
				if (isvalidwall(walltrack, orientation[0], r, c, size, color)) {
					walltrack[r][c]=orientation[0];
					historyupdate(&history, color, r, c, orientation[0]);
					printf("=\n\n");
				}
				else {
					printf("? illegal move\n\n");
				}
			}
		}
		else if (strcmp(arg, "undo")==0) {
			arg=strtok(NULL, " \n\t");
			if (arg==NULL) {
				i=1;
			}
			else {
				i=atoi(arg);
			}
			if (i==0) {
				printf("? syntax error\n\n");
			}
			else {
				undo(&history, walltrack, size, i);
				printf("=\n\n");
			}
		}
		else if (strcmp(arg, "genmove")==0) {
			getpath(size, walltrack);
			colortemp=strtok(NULL, " \n\t");
			if (colortemp==NULL) {
				printf("? syntax error\n\n");
				continue;
			}
			strcpy(color, colortemp);
			if (strcmp(color, "WHITE")==0 || strcmp(color, "w")==0 || strcmp(color, "W")==0) {
				strcpy(color, "white");
			}
			else if (strcmp(color, "BLACK")==0 || strcmp(color, "b")==0 || strcmp(color, "B")==0) {
				strcpy(color, "black");
			}
			if (strcmp(color, "white")==0 || strcmp(color, "black")==0) {
				treedepth=4;
				do {
					foundmove=0;
					//TODO: uncomment this. it is standard
				    //getmove(size, walltrack, color, 1, &wins, &sims, &chosenrow, &chosencol, &chosenori, treedepth, &foundmove, startvaluew, startvalueb);
					minMaxDecision(size, walltrack, 4, color, &chosenrow, &chosencol, &chosenori); foundmove = 1;
					treedepth+=2;
				}while(foundmove==0);
				printf("= %c%d", chosencol+'A', chosenrow+1);
				if (chosenori!=' ') {
					if (strcmp(color, "white")==0) {
						white.walls--;
					}
					else {
						black.walls--;
					}
					walltrack[chosenrow][chosencol]=chosenori;
					printf(" %c", chosenori);
				}
				else {
					updatepawn(color, chosenrow, chosencol);
				}
				historyupdate(&history, color, chosenrow, chosencol, chosenori);
				printf("\n");
				getpath(size, walltrack);
				printf("\n\n");
			}
			else {
				printf("? syntax error\n\n");
			}
		}
		else if (strcmp(arg, "winner")==0) {
			if (white.row==size-1) {
				printf("= true white\n\n");
			}
			else if (black.row==0) {
				printf("= true black\n\n");
			}
			else {
				printf("= false\n\n");
			}
		}
		else {
			printf("? unknown command\n\n");
		}
		fflush(stdout);
	}while (1);
	for (i=0;i<size;i++) {
		free(walltrack[i]);
	}
	free(walltrack);
	return 0;
}
