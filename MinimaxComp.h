/*
Connor A. Haskins
June 4th, 2016
CS420 -- Project 3: Four In A Line
*/

#ifndef MINIMAXCOMP
#define MINIMAXCOMP

#include "GameBoard.h"

/*
MinimaxComp is a class that helps the FourLineComputer in determining its opponents and
its own moves further down the line. It uses the Minimax algorithm, along with recursion,
to find the best possible move for the FourLineComputer.

[MinimaxComp functions could simply be added to FourLineComputer if desired]
*/
class MinimaxComp{
private:
	// does not need any persisting data
public:
	// constructors
	// creates a fresh gameboard
	MinimaxComp();

	// helps translate Minimax values
	char mToM(char mOrM);

	// user friendly function to determine the value of the current board
	char determineValue(char abPruneBest, int mDepth, GameBoard &gb);

	// function to determine the value of the current board
	char determineValue(bool min, char abPruneBest, int mDepth, GameBoard &gb);
};

#endif