void minpaths(int size, char** walltrack, int* minpathw, int* minpathb);
/*
	calls getpath to update minpath for both players, then returns minimum moves number to end for each player
	arguments: length of board size, table of wall positions, pointers to 2 ints and updating them to min path length for each player
	returns nothing, updates path tables through getpath and gets min paths for each player
*/
void getmove(int size, char** walltrack, char* color, int move, int* wins, int* sims, int*chosenr, int* chosenc, char* chosenori, int analdepth, int* foundmove, double startvaluew, double startvalueb);
/*
	-plays out potential moves for each player up to a tree depth of analdepth,
	 skipping moves that worsen position and stopping a move evaluation if an enemy move can
	 "counter" it (sort of alpha-beta pruning algorithm).
	 Values of chosenr, chosenc, choseori changed based on which "base" move gave the best value.
	-arguments: -length of board size
				-table containing wall placements
				-player to move ("white" or "black")
				-which move we are in (is called with 1 and in every recursion level is increased by 1)
				-pointer to total move simulations made from each 1st move. is automatically reseted on each 1st move checked
				-pointer to winning move simulations, works similarly to sims
				-pointers to chosen row, column and orientation.
				 Are given values based on best explored move
				 If move is not wall placement, orientation is ' '
				-search tree depth. Gets a value when calling from main
				 and it remains the same through the function. Must be even number
				-pointer to integer. A guard value in case getmove fails to find
				 a good enough move. Is passed a pointer to 0. If it stays 0 after
				 getmove, programmer is expected to react accordingly
				-2 doubles, the evaluation of the current position for white and black
				 Is updated for each function call and used to evaluate further moves
*/
