/*
Connor A. Haskins
June 4th, 2016
CS420 -- Project 3: Four In A Line
*/

#ifndef FOURLINECOMPUTER
#define FOURLINECOMPUTER

#include "GameBoard.h"

/*
FourLineComputer represents the computer opponent of the the Four In A Line game
it uses MinimaxComp and a GameBoard to help decide which is the next best move to make
It needs to be initialized with an integer that represents the time limit in seconds it has
available for each move
*/
class FourLineComputer{
private:
	int timeLimit;

public:
	// constructors
	// creates a fresh gameboard
	FourLineComputer(int timeLimit);

	// destructor
	//~FourLineComputer();

	bool move(GameBoard &gb);
};

#endif