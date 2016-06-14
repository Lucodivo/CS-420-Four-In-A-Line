/*
Connor A. Haskins
June 4th, 2016
CS420 -- Project 3: Four In A Line
*/

#ifndef GAMEBOARD
#define GAMEBOARD

#include <string>

#define BOARDWIDTH 8
#define BOARDSIZE (BOARDWIDTH * BOARDWIDTH)

/*
GameBoard class
Holds a string that represents the position of the board as a sequence of 64 chars
Allows users to place chars in any position on the board
if four characters are the same, the class will declare that the game is over
It also holds the formula that will create heuristic values for future moves
This is used simply to aid the FourLineComputer class in making its decisions
*/
class GameBoard{
private:
	// string that will hold the board, represented as 64 chars
	std::string board;

	// initialized to false, set to true when a player gets four in a line
	bool gameOver;

	// numSuccessMoves and isCatsGame are used to determine if it is a tie (no more legal moves)
	int numSuccessMoves;
	bool isCatsGame;

	// heuristic function that guesses the value of potential moves
	void calcMove(char c, int boardPos);

public:
	// constructors
	// creates a fresh gameboard
	GameBoard();

	// copy constructor
	GameBoard(const GameBoard &gb);

	// destructor
	~GameBoard();

	// accessors
	std::string getBoard();
	bool getGameOver();
	bool getCatsGame();

	// manipulators
	// used to place a character onto the board
	bool place(char c, int x, int y);

	// function used to display the board to the console
	void printBoard();
};

#endif