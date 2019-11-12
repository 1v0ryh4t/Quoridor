struct players {
	/*
	row: contains current row of pawn
	col: contains current column of pawn
	walls : walls available to each player
	pathing: array(2d), used to define paths to last row for each player
			each cell contains the number of moves needed to reach it
			or -1 if cell is unreachable
	*/
	int row, col, walls, minpath;
	char** pathing;
} white, black;

/*
	possiblemoves: array (2d) with all possible pawn moves for each player
	contains coordinants or -1 for unused array positions
*/
int possiblemoves[6][2];
