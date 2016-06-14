/*
Connor A. Haskins
June 4th, 2016
CS420 -- Project 3: Four In A Line
*/

#include "FourLineComputer.h"
#include "MinimaxComp.h"
#include <ctime>
#include <cmath>

/*
CONSTRUCTOR
the constructor for the FourLineComputer simply takes the time limit the AI will
take when making a decision for its next move
*/
FourLineComputer::FourLineComputer(int timeLimit){
	this->timeLimit = timeLimit;
}

/*
This is the main function of the AI, and works along side with a MinimaxComp object, in order
to recursively discover the best next move
Input: GameBoard &gb: A reference to a GameBoard. The GameBoard will not be changed at all, it will simply
be used as a way to create new GameBoards using the copy constructor
Output: a boolean value representing whether or not the move was successful (should always be successful
except in the position of a cat's game).
*/
bool FourLineComputer::move(GameBoard &gb){
	std::string board = gb.getBoard();

	// structure to hold time value at the beginning of the function call
	struct tm start;

	// acquire the initial time at the beginning of the function call
	time_t t = time(0);
	localtime_s(&start, &t);

	// save the seconds of the current time in an integer
	// this will be used to help determine when the function should return
	int startSeconds = start.tm_sec;

	// an integer representing the iterative depth of the current search for an optimal move
	int iDepth = 1;

	// the position for the "best" move
	// it is initialize to the middle of the board, in the event that the computer goes first 
	int bestPosition = (BOARDSIZE / 2) + (BOARDWIDTH / 2);

	// current best is sent to lowest char value
	char currentBest = '\0';

	// this integer will hold the difference between the end of each iteration and 
	// beginning of the entire function call
	int difference = 0;

	// structure to hold time value at the end of each iteration
	struct tm end;

	// create a new MinimaxComp
	MinimaxComp newMC;

	// while the difference has not exceeded the time limit
	while (difference < this->timeLimit && iDepth < 4){
		currentBest = '\0';

		// iterate through the entire board
		for (int i = 0; i < BOARDSIZE; i++){
			// if the current board position is a heuristically valid move
			if (board[i] < '9' && board[i] != '\0'){
				// create a copy of the current GameBoard
				GameBoard newGB(gb);
				// make the potential move
				newGB.place('X', (i % BOARDWIDTH), (i / BOARDWIDTH));
				// use the MinimaxComp object to discover a better heuristic value for the move
				char treeBest = newMC.determineValue(currentBest, iDepth, newGB);
				// if the new value is better than the current best
				if (treeBest > currentBest){
					// save it as current best value of the move
					currentBest = treeBest;
					// also, save the position in order to perform the move later
					bestPosition = i;
				}
			}
		}

		// if the move will result in a win, simply break the loop and perform that move
		if (currentBest == '5'){
			break;
		}

		// increase iterative depth
		iDepth++;

		// update time difference
		t = time(0);
		localtime_s(&end, &t);
		difference = abs(start.tm_sec - end.tm_sec) % 10;
	}

	// attemp to make the move and return the success value of the attempt
	return gb.place('X', (bestPosition % BOARDWIDTH), (bestPosition / BOARDWIDTH));
}