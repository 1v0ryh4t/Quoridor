char** createwalls(int r);
/*
	creates table storing walls and tables for paths of the same size
	arguments: length of board size. if it is <3, >25 or even number error
	message is thrown instead
	returns a pointer to the table for walls and automatically allocates
	space for pathing tables
*/
void clr(char** walls, int size);
/*
	resets pathing and wall tables
	arguments: pointer to 2dim wall array and length of board size
	returns nothing, pathing table cells reset to -1, walls table cells
	reset to ' '
*/
void show(char** walls, int size);
/*
	prints table as it is formated at time being (with pawn positions)
	arguments: pointer to table array, length of board size
	returns nothing, prints table step by step, following this pattern:
	Pawns shown with W and B accordingly
	generally "borders" between cells are - and | and + in "diagonal" borders
	if there is a wall between cells + is replaced with - or | accordingly
*/
void setpawns(int size);
/*
	pawn positions set to start (middle top, middle bottom)
	arguments: length of board size
	returns nothing, white/black.row, white/black.col values changed
	acccordingly
*/
