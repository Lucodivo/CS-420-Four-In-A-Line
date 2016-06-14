/*
Connor A. Haskins
June 4th, 2016
CS420 -- Project 3: Four In A Line
*/

#include "MinimaxComp.h"
#include "GameBoard.h"

/*
CONSTRUCTOR
no data, nothing needs initialization
*/
MinimaxComp::MinimaxComp(){
}

/*
user friendly version of determine value
INPUT:	char abPruneBest - current best value (before determineValue is called),
used alpha-beta pruning
int mDepth - the maximum depth (ply) we want minimax to go
GameBoard &gb - a reference to an existing board with the desired move whose
value needs to be determined.
OUTPUT: a char representing the quality of the move (represented from [0-5])
*/
char MinimaxComp::determineValue(char abPruneBest, int mDepth, GameBoard &gb){
	return this->determineValue(true, abPruneBest, mDepth, gb);
}

/*
determineValue uses the Minimax algorithm to determine a heuristic value of a particular
board (a board in which a particular movement is made). In order to use the Minimax algorithm,
the function calls itself recursively and is able to use the same function for both the Min
and the Max players. Both Min and Max are actually attempting to get the greatest values and, thus,
both behave like Max. This is solved by "inverting" the determined values everytime Min and Max communicate
through returning chars. This allows us to essentially have both the Min and Max act like Max while treating the
opposing player as if it is a Min.

INPUT:	char abPruneBest - current best value (before determineValue is called),
used alpha-beta pruning
int mDepth - the maximum depth (ply) we want minimax to go
GameBoard &gb - a reference to an existing board with the desired move whose
value needs to be determined.
OUTPUT: a char representing the quality of the move (represented from [0-5], [worst-best])
*/
char MinimaxComp::determineValue(bool min, char abPruneBest, int mDepth, GameBoard &gb){
	// initialize the current best as lowest char value
	char best = '\0';

	// the abPruneBest will get passed to a min from a max or to a max from a min
	// so we need to translate that value between the two
	abPruneBest = this->mToM(abPruneBest);

	// player symbol needs to toggle between recursive calls
	char playerSymbol;

	// if min is true, we want to place 'O' chars
	if (min) {
		playerSymbol = 'O';
	}
	// else, place 'X' chars
	else {
		playerSymbol = 'X';
	}

	// get the board from the GameBoard argument
	std::string board = gb.getBoard();

	// if the current board is currently a GameOver
	if (gb.getGameOver()){
		// the current player has lost, set the value to '0' representing that loss (it will return a '5' after translated)
		best = '0';
	}
	// if the current mDepth is 0, stop recursion and simply use the heuristic board values
	else if (mDepth == 0){
		// for ever element of the board
		for (int i = 0; i < BOARDSIZE; i++){
			// if the element is greater than the current best and is not 'X' or 'O'
			if (board[i] > best && board[i] < '9'){
				// set the best as the current element
				best = board[i];
				// if the move is better than the prune value, we can stop evaluating and break the loop
				if (best == '4' || (best >= abPruneBest && abPruneBest != '\0')){
					// break out of for loop
					break;
				}
			}
		}
	}
	// if the mDepth is greater than zero (should never be negative)
	else {
		// for ever element of the board
		for (int i = 0; i < BOARDSIZE; i++){
			// only evaluate positions where a heuristic value exists 
			if (board[i] < '9' && board[i] != '\0'){
				// create a new copy of the GameBoard
				GameBoard newGB(gb);

				// perform the current move
				newGB.place(playerSymbol, (i % BOARDWIDTH), (i / BOARDWIDTH));

				// determine the value of the current move using a recursive call to
				// determineValue. Send it the opposite value of min to toggle between
				// min and max, send it the current best to be used in alpha-beta pruning
				// send a decremented depth, and the new GameBoard with the new move
				char potBest = this->determineValue(!min, best, (mDepth - 1), newGB);

				// if the determined value is better than the current best
				if (potBest > best){
					// update the value of the current best
					best = potBest;
					// if the potential best is better than the PruneBest, we need to prune
					// and stop evaluating the remaining branches
					if ((potBest >= abPruneBest && abPruneBest != '\0') || potBest == '5'){
						// break out of for loop
						break;
					}
				}
			}
		}
	}

	// we need to return a translation of the determined value
	return this->mToM(best);
}

/*
the min and max will both be using the same values
In order to represent a "minimum" and a "maximum"
we must be able to translate between the two. Since
both "players" are attempting to achieve a maximum
score, we simply need a good move from Max to seem
like a bad move for Min and vice versa. This function
helps implement that idea.

if input is a maximum value, it will be translated
to a useful value for the minimum.

if input is a minimum value, it will be translated
to a useful value for the maximum.

INPUT: char mOrM - the value to be translated
OUTPUT: a char representing the translated value
*/
char MinimaxComp::mToM(char mOrM){
	switch (mOrM){
	case '/0':
		return mOrM;
	case '0':
		return '5';
	case '1':
		return '4';
	case '2':
		return '3';
	case '3':
		return '2';
	case '4':
		return '1';
	case '5':
		return '0';
	}
}