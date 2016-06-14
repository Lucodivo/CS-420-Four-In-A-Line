/*
Connor A. Haskins
June 4th, 2016
CS420 -- Project 3: Four In A Line
*/

#include "GameBoard.h"
#include <iostream>

/*
CONSTRUCTOR
string board: a string that will hold the 'X' and 'O' player moves
int numSuccessMoves: an integer that holds number of successful moves on current board
Used strictly to determine a cat's game (tie)
bool isCatsGame: initially set to false, only set to true if numSuccessMoves reaches 64
bool gameOver: initially set to false, only set when one player wins (four in a line)
*/
GameBoard::GameBoard(){
	this->board = std::string(BOARDSIZE, '\0');
	this->numSuccessMoves = 0;
	this->isCatsGame = false;
	this->gameOver = false;
}

/*
COPY CONSTRUCTOR
simply copys all values from passed GameBoard's members to the new GameBoard's members
*/
GameBoard::GameBoard(const GameBoard &gb){
	this->board = std::string(gb.board);
	this->numSuccessMoves = gb.numSuccessMoves;
	this->isCatsGame = gb.isCatsGame;
	this->gameOver = gb.gameOver;
}

/*
DESTRUCTOR (NO LONGER USED)
was used when I was allocating memory for a char array
also proved to be useful for tracking the amount of boards being created
*/
GameBoard::~GameBoard(){
}

// board getter
std::string GameBoard::getBoard(){
	return this->board;
}

// gameOver getter
bool GameBoard::getGameOver(){
	return this->gameOver;
}

// isCatsGame getter
bool GameBoard::getCatsGame(){
	return this->isCatsGame;
}

/*
This functions main purpose is to check if the move is legal and if it is, call the calcMove() in order to
create heurstic values for FourLineComputer.

INPUT: char c - the character to be written to the board (should really only be 'X' or 'O')
int x - the x position (column) of the desired placement of the character
int y - the y position (row) of the desired placement of the character
OUTPUT: a boolean value representing the success of the placemen (true = placement succeeded, false = illegal move)
*/
bool GameBoard::place(char c, int x, int y){
	// calculate the index of the desired move
	int boardPos = (y * BOARDWIDTH) + x;

	if (x > -1 && y > -1 && x < BOARDWIDTH && y < BOARDWIDTH && this->board[boardPos] != 'X' && this->board[boardPos] != 'O'){
		// set the position on the board to the desired character
		this->board[boardPos] = c;

		// calculate if the move won the game and how it affected the board
		this->calcMove(c, boardPos);

		// increment the number of successful moves
		if (++numSuccessMoves == 64){
			this->isCatsGame = true;
		}

		// return that the move was successful
		return true;
	}
	// else move is illegal, no move was made, return false
	return false;
}


/*
Aside from determining the winner, this function's main purpose is too store information about the value of
potential moves for the FourLineComputer object. It categorizes empty positions with a legend of five different
values (OFFENSIVE AND DEFENSIVE MOVES ARE TREATED EQUALLY):
0 - Should never lead to a win, no need for defensive action
1 - Nothing good or bad about the move, solid offense/defense if nothing else
2 - Should lead to an L-shape pattern, fantastic offense or defense but depends on higher values
3 - Should lead to a guaranteed win on the next turn, also depends on higher value
4 - Potential win OR potential prevention of win

This function has A LOT of pattern matching equations and is the main heuristics that aid in FourLineComputer
assessing all non-termal nodes in the tree of potential moves. This function could also use massive amounts of
optimization.

INPUT:	char c - the character (usually 'X' or 'O') that is being entered onto the board
int boardPos - the position that character is being entered onto the board
OUTPUT: void
*/
void GameBoard::calcMove(char c, int boardPos){

	// calculate the current x- and y-values
	int xPos = boardPos % BOARDWIDTH;
	int yPos = boardPos / BOARDWIDTH;

	// distances from upper wall (3 squares, 2 squares, 1 square)
	bool uDistance3 = (yPos > 2);
	bool uDistance2 = uDistance3 || (yPos > 1);
	bool uDistance1 = uDistance2 || (yPos > 0);

	// distances from lower wall (3 squares, 2 squares, 1 square)
	bool dDistance3 = (yPos < 5);
	bool dDistance2 = dDistance3 || (yPos < 6);
	bool dDistance1 = dDistance2 || (yPos < 7);

	// distances from left wall (3 squares, 2 squares, 1 square)
	bool lDistance3 = (xPos > 2);
	bool lDistance2 = lDistance3 || (xPos > 1);
	bool lDistance1 = lDistance2 || (xPos > 0);

	// distances from right wall (3 squares, 2 squares, 1 square)
	bool rDistance3 = ((boardPos % 8) < 5);
	bool rDistance2 = rDistance3 || (xPos < 6);
	bool rDistance1 = rDistance2 || (xPos < 7);

	// adjacent positions to the one being added
	int uAdjPos1 = boardPos - BOARDWIDTH;	// up1
	int uAdjPos2 = uAdjPos1 - BOARDWIDTH;	// up2
	int uAdjPos3 = uAdjPos2 - BOARDWIDTH;	// up3
	int dAdjPos1 = boardPos + BOARDWIDTH;	// down1
	int dAdjPos2 = dAdjPos1 + BOARDWIDTH;	// down2
	int dAdjPos3 = dAdjPos2 + BOARDWIDTH;	// down3
	int lAdjPos1 = boardPos - 1;			// left1
	int lAdjPos2 = lAdjPos1 - 1;			// left2
	int lAdjPos3 = lAdjPos2 - 1;			// left3
	int rAdjPos1 = boardPos + 1;			// right1
	int rAdjPos2 = rAdjPos1 + 1;			// right2
	int rAdjPos3 = rAdjPos2 + 1;			// right3

	// U1 is adjacent up, U2 is adjacent up to U1, U3 is adjacent up to U2
	bool U1 = uDistance1 && (this->board[uAdjPos1] == c);
	bool U2 = U1 && uDistance2 && (this->board[uAdjPos1 - BOARDWIDTH] == c);
	bool U3 = U2 && uDistance3 && (this->board[uAdjPos1 - (2 * BOARDWIDTH)] == c);

	// D1 is adjacent down, D2 is adjacent down to D1, D3 is adjacent down to D2
	bool D1 = dDistance1 && (this->board[dAdjPos1] == c);
	bool D2 = D1 && dDistance2 && (this->board[dAdjPos1 + BOARDWIDTH] == c);
	bool D3 = D2 && dDistance3 && (this->board[dAdjPos1 + (2 * BOARDWIDTH)] == c);

	// L1 is adjacent left, L2 is adjacent left to L1, L3 is adjacent left to L2
	bool L1 = lDistance1 && (this->board[lAdjPos1] == c);
	bool L2 = L1 && lDistance2 && (this->board[lAdjPos1 - 1] == c);
	bool L3 = L2 && lDistance3 && (this->board[lAdjPos1 - 2] == c);

	// R1 is adjacent right, R2 is adjacent right to R1, R3 is adjacent right to R2
	bool R1 = rDistance1 && (this->board[rAdjPos1] == c);
	bool R2 = R1 && rDistance2 && (this->board[rAdjPos1 + 1] == c);
	bool R3 = R2 && rDistance3 && (this->board[rAdjPos1 + 2] == c);

	// check if the game has ended
	if (L3 || (L2 && R1) || (L1 && R2) || R3 ||
		D3 || (D2 && U1) || (D1 && U2) || U3){
		this->gameOver = true;
	}
	// if not game over, we want to continue calculating the heuristic values of the remaining legal moves
	else {

		// if opposing sides of the new placement are free and they are currently at a value < 1,
		// set their value to 1
		/*
		1				|		  0
		1 X 1			|		1 X 1
		1				|		  O
		*/
		if (uDistance1 && this->board[uAdjPos1] < '1'){
			if (dDistance1 && this->board[dAdjPos1] < '9'){
				this->board[uAdjPos1] = '1';
			}
			else {
				this->board[uAdjPos1] = '0';
			}
		}
		if (dDistance1 && this->board[dAdjPos1] < '1'){
			if (uDistance1 && this->board[uAdjPos1] < '9'){
				this->board[dAdjPos1] = '1';
			}
			else {
				this->board[dAdjPos1] = '0';
			}
		}
		if (lDistance1 && this->board[lAdjPos1] < '1'){
			if (rDistance1 && this->board[rAdjPos1] < '9'){
				this->board[lAdjPos1] = '1';
			}
			else {
				this->board[lAdjPos1] = '0';
			}
		}
		if (rDistance1 && this->board[rAdjPos1] < '1'){
			if (lDistance1 && this->board[lAdjPos1] < '9'){
				this->board[rAdjPos1] = '1';
			}
			else {
				this->board[rAdjPos1] = '0';
			}
		}

		// positions in "adjacent" diagonal to currently placed character
		int uLeftPos = uAdjPos1 - 1;
		int uRightPos = uAdjPos1 + 1;
		int dLeftPos = dAdjPos1 - 1;
		int dRightPos = dAdjPos1 + 1;

		// calculate if diagonal positions are equal to current positions character
		bool Dul = uDistance1 && lDistance1 && (this->board[uLeftPos] == c);
		bool Dur = uDistance1 && rDistance1 && (this->board[uRightPos] == c);
		bool Ddl = dDistance1 && lDistance1 && (this->board[dLeftPos] == c);
		bool Ddr = dDistance1 && rDistance1 && (this->board[dRightPos] == c);

		// calculate heuristic values for potential 'L-shape' moves
		/*
		1			=>	  1
		1 X 1		=>	1 X 2
		1			=>	  2 X 1
		1
		*/
		if ((Dur || Dul) && this->board[uAdjPos1] < '2') {
			this->board[uAdjPos1] = '2';
		}
		if ((Dul || Ddl) && this->board[lAdjPos1] < '2') {
			this->board[lAdjPos1] = '2';
		}
		if ((Ddl || Ddr) && this->board[dAdjPos1] < '2') {
			this->board[dAdjPos1] = '2';
		}
		if ((Ddr || Dur) && this->board[rAdjPos1] < '2') {
			this->board[rAdjPos1] = '2';
		}

		// determine the heuristic of two equal characters in a row
		/*
		1		=>		  3			|	  		=>		  1				|		  1			=>		  3
		1 X 1		=>		2 X 2		|	  1		=>		2 X 2			|		O X 1		=>		O X 1
		1		=>		2 X 2		|	1 X 1	=>		2 X 2			|		O 1			=>		O X 1
		3			|	  O		=>		  0				|					=>		  3
		*/
		if (U1){
			if (dDistance1 && this->board[dAdjPos1] < '3'){
				if (uDistance2 && this->board[uAdjPos2] < '9'){
					this->board[dAdjPos1] = '3';
				}
				else {
					this->board[dAdjPos1] = '1';
				}
			}
			if (uDistance2 && this->board[uAdjPos2] < '3'){
				if (dDistance1 && this->board[dAdjPos1] < '9'){
					this->board[uAdjPos2] = '3';
				}
				else {
					this->board[uAdjPos2] = '1';
				}
			}

			if (lDistance1){
				if (!rDistance1 || (!R1 && this->board[rAdjPos1] > '9')){
					if (this->board[lAdjPos1] < '3'){
						this->board[lAdjPos1] = '0';
					}
					else if (this->board[lAdjPos1] < '9'){
						this->board[lAdjPos1] = '3';
					}
				}
				else if (this->board[lAdjPos1] < '2'){
					this->board[lAdjPos1] = '2';
				}

				if (!rDistance1 || (!Dur && this->board[uRightPos] > '9')){
					if (this->board[uLeftPos] < '3'){
						this->board[uLeftPos] = '0';
					}
					else if (this->board[uLeftPos] < '9'){
						this->board[uLeftPos] = '3';
					}
				}
				else if (this->board[uLeftPos] < '2'){
					this->board[uLeftPos] = '2';
				}
			}
			if (rDistance1){
				if (!lDistance1 || (!L1 && this->board[lAdjPos1] > '9')){
					if (this->board[rAdjPos1] < '3'){
						this->board[rAdjPos1] = '0';
					}
					else if (this->board[rAdjPos1] < '9'){
						this->board[rAdjPos1] = '3';
					}
				}
				else if (this->board[rAdjPos1] < '2'){
					this->board[rAdjPos1] = '2';
				}

				if (!lDistance1 || (!Dul && this->board[uLeftPos] > '9')){
					if (this->board[uRightPos] < '3'){
						this->board[uRightPos] = '0';
					}
					else if (this->board[uRightPos] < '9'){
						this->board[uRightPos] = '3';
					}
				}
				else if (this->board[uRightPos] < '2'){
					this->board[uRightPos] = '2';
				}
			}
		}
		if (D1){
			if (uDistance1 && this->board[uAdjPos1] < '3'){
				if (dDistance2 && this->board[dAdjPos2] < '9'){
					this->board[uAdjPos1] = '3';
				}
				else {

					this->board[uAdjPos1] = '1';
				}
			}
			if (dDistance2 && this->board[dAdjPos2] < '3'){
				if (uDistance1 && this->board[uAdjPos1] < '9'){
					this->board[dAdjPos2] = '3';
				}
				else {
					this->board[dAdjPos2] = '1';
				}
			}

			if (lDistance1){
				if (!rDistance1 || (!R1 && this->board[rAdjPos1] > '9')){
					if (this->board[lAdjPos1] < '3'){
						this->board[lAdjPos1] = '0';
					}
					else if (this->board[lAdjPos1] < '9'){
						this->board[lAdjPos1] = '3';
					}
				}
				else if (this->board[lAdjPos1] < '2'){
					this->board[lAdjPos1] = '2';
				}

				if (!rDistance1 || (!Ddr && this->board[dRightPos] > '9')){
					if (this->board[dLeftPos] < '3'){
						this->board[dLeftPos] = '0';
					}
					else if (this->board[dLeftPos] < '9'){
						this->board[dLeftPos] = '3';
					}
				}
				else if (this->board[dLeftPos] < '2'){
					this->board[dLeftPos] = '2';
				}
			}

			if (rDistance1){
				if (!lDistance1 || (!L1 && this->board[lAdjPos1] > '9')){
					if (this->board[rAdjPos1] < '3'){
						this->board[rAdjPos1] = '0';
					}
					else if (this->board[rAdjPos1] < '9'){
						this->board[rAdjPos1] = '3';
					}
				}
				else if (this->board[rAdjPos1] < '2'){
					this->board[rAdjPos1] = '2';
				}

				if (!lDistance1 || (!Ddl && this->board[dLeftPos] > '9')){
					if (this->board[dRightPos] < '3'){
						this->board[dRightPos] = '0';
					}
					else if (this->board[dRightPos] < '9'){
						this->board[dRightPos] = '3';
					}
				}
				else if (this->board[dRightPos] < '2'){
					this->board[dRightPos] = '2';
				}
			}
		}
		if (L1){
			if (rDistance1 && this->board[rAdjPos1] < '3'){
				if (lDistance2 && this->board[lAdjPos2] < '9'){
					this->board[rAdjPos1] = '3';
				}
				else {
					this->board[rAdjPos1] = '1';
				}
			}
			if (lDistance2 && this->board[lAdjPos2] < '3'){
				if (rDistance1 && this->board[rAdjPos1] < '9'){
					this->board[lAdjPos2] = '3';
				}
				else {
					this->board[lAdjPos2] = '1';
				}
			}

			if (uDistance1){
				if (!dDistance1 || (!D1 && this->board[dAdjPos1] > '9')){
					if (this->board[uAdjPos1] < '3'){
						this->board[uAdjPos1] = '0';
					}
					else if (this->board[uAdjPos1] < '9'){
						this->board[uAdjPos1] = '3';
					}
				}
				else if (this->board[uAdjPos1] < '2'){
					this->board[uAdjPos1] = '2';
				}

				if (!dDistance1 || (!Ddl && this->board[dLeftPos] > '9')){
					if (this->board[uLeftPos] < '3'){
						this->board[uLeftPos] = '0';
					}
					else if (this->board[uLeftPos] < '9'){
						this->board[uLeftPos] = '3';
					}
				}
				else if (this->board[uLeftPos] < '2'){
					this->board[uLeftPos] = '2';
				}
			}
			if (dDistance1){
				if (!uDistance1 || (!U1 && this->board[uAdjPos1] > '9')){
					if (this->board[dAdjPos1] < '3'){
						this->board[dAdjPos1] = '0';
					}
					else if (this->board[dAdjPos1] < '9'){
						this->board[dAdjPos1] = '3';
					}
				}
				else if (this->board[dAdjPos1] < '2'){
					this->board[dAdjPos1] = '2';
				}

				if (!uDistance1 || (!Dul && this->board[uLeftPos] > '9')){
					if (this->board[dLeftPos] < '3'){
						this->board[dLeftPos] = '0';
					}
					else if (this->board[dLeftPos] < '9'){
						this->board[dLeftPos] = '3';
					}
				}
				else if (this->board[dLeftPos] < '2'){
					this->board[dLeftPos] = '2';
				}
			}
		}
		if (R1){
			if (lDistance1 && this->board[lAdjPos1] < '3'){
				if (rDistance2 && this->board[rAdjPos2] < '9'){
					this->board[lAdjPos1] = '3';
				}
				else {
					this->board[lAdjPos1] = '1';
				}
			}
			if (rDistance2 && this->board[rAdjPos2] < '3'){
				if (lDistance1 && this->board[lAdjPos1] < '9'){
					this->board[rAdjPos2] = '3';
				}
				else {
					this->board[rAdjPos2] = '1';
				}
			}

			if (uDistance1){
				if (!dDistance1 || (!D1 && this->board[dAdjPos1] > '9')){
					if (this->board[uAdjPos1] < '3'){
						this->board[uAdjPos1] = '0';
					}
					else if (this->board[uAdjPos1] < '9'){
						this->board[uAdjPos1] = '3';
					}
				}
				else if (this->board[uAdjPos1] < '2'){
					this->board[uAdjPos1] = '2';
				}

				if (!dDistance1 || (!Ddr && this->board[dRightPos] > '9')){
					if (this->board[uRightPos] < '3'){
						this->board[uRightPos] = '0';
					}
					else if (this->board[uRightPos] < '9'){
						this->board[uRightPos] = '3';
					}
				}
				else if (this->board[uRightPos] < '2'){
					this->board[uRightPos] = '2';
				}
			}

			if (dDistance1){
				if (!uDistance1 || (!U1 && this->board[uAdjPos1] > '9')){
					if (this->board[dAdjPos1] < '3'){
						this->board[dAdjPos1] = '0';
					}
					else if (this->board[dAdjPos1] < '9'){
						this->board[dAdjPos1] = '3';
					}
				}
				else if (this->board[dAdjPos1] < '2'){
					this->board[dAdjPos1] = '2';
				}

				if (!uDistance1 || (!Dur && this->board[uRightPos] > '9')){
					if (this->board[dRightPos] < '3'){
						this->board[dRightPos] = '0';
					}
					else if (this->board[dRightPos] < '9'){
						this->board[dRightPos] = '3';
					}
				}
				else if (this->board[dRightPos] < '2'){
					this->board[dRightPos] = '2';
				}
			}
		}

		/*
		1					=>		  1
		1 X 1				=>		1 X 1
		1					=>		  3
		=>		1 X 1
		=>		  1
		*/
		if (uDistance2 && this->board[uAdjPos2] == c){
			if (((uDistance3 && this->board[uAdjPos3] < '9') || (dDistance1 && this->board[dAdjPos1] < '9')) && this->board[uAdjPos1] < '4'){
				this->board[uAdjPos1] = '3';
			}
		}
		if (dDistance2 && this->board[dAdjPos2] == c){
			if (((dDistance3 && this->board[dAdjPos3] < '9') || (uDistance1 && this->board[uAdjPos1] < '9')) && this->board[dAdjPos1] < '4'){
				this->board[dAdjPos1] = '3';
			}
		}
		if (lDistance2 && this->board[lAdjPos2] == c){
			if (((lDistance3 && this->board[lAdjPos3] < '9') || (rDistance1 && this->board[rAdjPos1] < '9')) && this->board[lAdjPos1] < '4'){
				this->board[lAdjPos1] = '3';
			}
		}
		if (rDistance2 && this->board[rAdjPos2] == c){
			if (((rDistance3 && this->board[rAdjPos3] < '9') || (lDistance1 && this->board[lAdjPos1] < '9')) && this->board[rAdjPos1] < '4'){
				this->board[rAdjPos1] = '3';
			}
		}

		// calculate potential winning positions
		/*
		3			=>		  4
		2 X 2		=>		2 X 2
		2 X 2		=>		2 X 2
		3			=>		2 X 2
		4
		*/
		if (U2){
			if (dDistance1 && this->board[dAdjPos1] < '4'){
				this->board[dAdjPos1] = '4';
			}
			if (uDistance3 && this->board[uAdjPos3] < '4'){
				this->board[uAdjPos3] = '4';
			}
		}
		if (D2){
			if (uDistance1 && this->board[uAdjPos1] < '4'){
				this->board[uAdjPos1] = '4';
			}
			if (dDistance3 && this->board[dAdjPos3] < '4'){
				this->board[dAdjPos3] = '4';
			}
		}
		if (L2){
			if (rDistance1 && this->board[rAdjPos1] < '4'){
				this->board[rAdjPos1] = '4';
			}
			if (lDistance3 && this->board[lAdjPos3] < '4'){
				this->board[lAdjPos3] = '4';
			}
		}
		if (R2){
			if (lDistance1 && this->board[lAdjPos1] < '4'){
				this->board[lAdjPos1] = '4';
			}
			if (rDistance3 && this->board[rAdjPos3] < '4'){
				this->board[rAdjPos3] = '4';
			}
		}

		/*
		1			=>		  4
		1 X 1		=>		2 X 2
		2			=>		2 X 2
		1 X 1		=>		2 X 2
		1			=>		  4
		*/
		if (U1 && D1) {
			if (uDistance2 && this->board[uAdjPos2] < '4'){
				this->board[uAdjPos2] = '4';
			}
			if (dDistance2 && this->board[dAdjPos2] < '4'){
				this->board[dAdjPos2] = '4';
			}
		}
		if (L1 && R1) {
			if (lDistance2 && this->board[lAdjPos2] < '4'){
				this->board[lAdjPos2] = '4';
			}
			if (rDistance2 && this->board[rAdjPos2] < '4'){
				this->board[rAdjPos2] = '4';
			}
		}

		/*
		3			=>		  3
		2 X 2		=>		2 X 2
		2 X 2		=>		2 X 2
		3			=>		  4
		=>		1 X 1
		=>		  1
		*/
		if (uDistance3 && this->board[uAdjPos2] == c && this->board[uAdjPos3] == c && this->board[uAdjPos1] < '9'){
			this->board[uAdjPos1] = '4';
		}
		if (dDistance3 && this->board[dAdjPos2] == c && this->board[dAdjPos3] == c && this->board[dAdjPos1] < '9'){
			this->board[dAdjPos1] = '4';
		}
		if (lDistance3 && this->board[lAdjPos2] == c && this->board[lAdjPos3] == c && this->board[lAdjPos1] < '9'){
			this->board[lAdjPos1] = '4';
		}
		if (rDistance3 && this->board[rAdjPos2] == c && this->board[rAdjPos3] == c && this->board[rAdjPos1] < '9'){
			this->board[rAdjPos1] = '4';
		}

		/*
		3
		1			=>		2 X 2
		1 X 1		=>		2 X 2
		3			=>		  4
		1 X	1		=>		1 X 1
		1			=>		  1
		*/
		if (dDistance3 && D1 && this->board[dAdjPos3] == c && this->board[dAdjPos2] < '9'){
			this->board[dAdjPos2] = '4';
		}
		if (uDistance3 && U1 && this->board[uAdjPos3] == c && this->board[uAdjPos2] < '9'){
			this->board[uAdjPos2] = '4';
		}
		if (lDistance3 && L1 && this->board[lAdjPos3] == c && this->board[lAdjPos2] < '9'){
			this->board[lAdjPos2] = '4';
		}
		if (rDistance3 && R1 && this->board[rAdjPos3] == c && this->board[rAdjPos2] < '9'){
			this->board[rAdjPos2] = '4';
		}

		/*
		1					  3
		1 X	1		=>		2 X 2
		1  		=>		2 X 2
		1			=>		  4
		1 X	1		=>		1 X 1
		1			=>		  1
		*/
		if (dDistance2 && U1 && this->board[dAdjPos2] == c && this->board[dAdjPos1] < '9'){
			this->board[dAdjPos1] = '4';
		}
		if (uDistance2 && D1 && this->board[uAdjPos2] == c && this->board[uAdjPos1] < '9'){
			this->board[uAdjPos1] = '4';
		}
		if (lDistance2 && R1 && this->board[lAdjPos2] == c && this->board[lAdjPos1] < '9'){
			this->board[lAdjPos1] = '4';
		}
		if (rDistance2 && L1 && this->board[rAdjPos2] == c && this->board[rAdjPos1] < '9'){
			this->board[rAdjPos1] = '4';
		}
	}
}

/*
A function that simply prints out the game board to the console
INPUT: void
OUTPUT: void
*/
void GameBoard::printBoard(){
	// print out the initial row (column names)
	std::cout << "   1 2 3 4 5 6 7 8" << std::endl;
	// for every row
	for (int i = 0; i < BOARDWIDTH; i++){ // i acts as y-value
		// print the row name...
		std::cout << " " << (char)('A' + i);
		// followed by either '-' or the current placements of 'X' and 'O'
		for (int j = 0; j < BOARDWIDTH; j++){ // j acts x-value
			std::cout << " ";
			int currentPos = (i * BOARDWIDTH) + j;
			// only print parts of the board if they are 'X' or 'Y' (uncomment the line inside the if statement to display heuristic values)
			if (this->board[currentPos] == 'X' || this->board[currentPos] == 'O'/* || this->board[currentPos] != '\0'*/){
				std::cout << this->board[currentPos];
			}
			else {
				std::cout << "-";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}