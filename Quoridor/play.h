typedef struct hlist *History;

/*
2-way (previous and next pointer) list
contains color that moves, coordinants of the move and orientation (' ' for pawn move, 'H', 'V')
for each hlist element
*/

struct hlist {
	History previous;
	char color[6], orientation;
	int row, col;
	History next;
};

History history;

int isvalidmove(char** walltrack, int size, int r, int c, char* color);
/* checks the validity of a pawn move
arguments: board of walls, size of board, coordinants of move, playing color
returns 1 if move is valid, 0 otherwise (and respective message)
*/

void getvalidmoves(int size, char** walltrack, char* color, int r, int c);
/* gets all valid pawn moves for the color specified
arguments: size of board, board of walls, color specified, current row and column of pawn to move
returns nothing, fixes an array with the valid moves
*/

void updatepawn(char* color, int r, int c);
/* updates position of pawn of specified color
arguments: color of pawn, row and column where it will be placed
returns nothing, updates specified row and col variables from players struct
*/

void undo(History *history, char** walltrack, int size, int times);
/* gets board to a previous state
arguments: address of hlist pointer, board of walls, size of board, times to undo
returns nothing, updates hlist
*/

void getpath(int size, char** walltrack);
/* gets the moves needed for each player to reach a cell
arguments: size of board, board of walls
returns nothing, updates pathing arrays in struct players
*/
void historyupdate(History *history, char* color, int r, int c, char orientation);
/* updates history after a move is made
arguments: address of hlist pointer, color to make move, coordinants of move, orientation (' ' if it was a pawn move, otherwise 'H' or 'V')
returns nothing, updates hlist with the move
*/
void clrhistory(History *history);
/* resets hlist
arguments: address of hlist pointer
returns nothing, clears hlist, making the pointer to hlist=NULL
*/
void printhistory (History *history);
/* prints hlist
arguments: address of pointer to hlist
returns nothing, prints the history of the game
history is printed in template: COLOR, ,COLUMN,ROW, ,ORIENTATION
where each template element is specified from historyupdate
*/
int isvalidwall(char** walltrack, char orientation, int r, int c, int size, char* color);
/* checks the validity of a wall
arguments: board of walls, orientation of requested wall, coordinants of wall, size of board, color to place the wall
returns 1 if wall is valid, 0 if it is invalid
wall is invalid if there are no available walls for player requesting it
				if it gets out of bounds
				if it blocks a player's path to the last row
*/
