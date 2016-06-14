/*
Connor A. Haskins
June 4th, 2016
CS420 -- Project 3: Four In A Line
*/

#include "GameBoard.h"
#include "FourLineComputer.h"
#include <iostream>
#include <ctime>

/*
The main function for the Four In A Line game
Gameboard, FourLineComputer, and MinimaxComp classes are all necessary for the program to work

Functionality:
- User can play a single player or two player game of Four In A Line
- if they play a two player game, they will play with the FourLineComputer AI
- the will enter the desired column and row values for their moves until the
game comes to an end
- the game prints out the value of the board each step of the way
*/

int main(){
	// the main GameBoard that will be used for the duration of the game
	GameBoard gb;

	// player's movement positions: X is the column(represented by an int), Y is the row(represented by a char)
	int playerX;
	char playerY;

	// toggle between 'O' (Human/true) and 'X' (Computer/false)
	bool playerOneToggle = true;

	// whether we want a computer player or not
	bool computerPlayer = true;

	// used to determine whether the player wants to play with a computer 
	// ...and whether they want to play first or second
	// initialized to 'y', assumed that the player wants to play first
	char playerChoice = 'y';

	// also get a response on whether the player wants to play with a computer (or just 1v1 with friend)
	std::cout << "Would you like to play with a computer? (y/n): ";
	std::cin >> playerChoice;
	std::cout << std::endl;

	// if the answer is 'n' or 'N', there is no computer. Else there is a computer player
	if (playerChoice == 'n' || playerChoice == 'N'){
		computerPlayer = false;
	}

	int numSeconds = 5;

	if (computerPlayer){
		// get response on whether player wants to go first or second
		std::cout << "Would you like to go first? (y/n): ";
		std::cin >> playerChoice;
		std::cout << std::endl;

		// if the answer is 'n' or 'N', they go second. Else they will play first
		if (playerChoice == 'n' || playerChoice == 'N'){
			playerOneToggle = false;
		}

		// get the amount of time the computer should be able to think about its next move
		std::cout << "How long should the computer think about its moves (in seconds)? : ";
		std::cin >> numSeconds;
		std::cout << std::endl;
	}

	// instantiate our FourLineComputer AI that the opponent will play against
	FourLineComputer flc(numSeconds);

	// print initial board
	gb.printBoard();

	// the game loop, will continue until game is over
	while (!gb.getGameOver() && !gb.getCatsGame()){
		// boolean used to determine whether the attempted move was successful
		bool successfulMove = false;
		// if it is playerOne's move...
		if (playerOneToggle){
			// get the next move from the player as a char and int (ex: d5)
			std::cout << "Choose your next move: ";
			std::cin >> playerY;
			std::cin >> playerX;
			// attempt to perform the move and store the returned value in successfulMove
			// the return value is true if the move was legal and false if it wasn't
			successfulMove = gb.place('O', (playerX - 1), (int)(playerY - 97));
		}
		// else it is player 2's turn
		else {
			// if there is a computer playing
			if (computerPlayer){
				// tell the computer to perform the next move by calling move(GameBoard gb) 
				// and passing it the current GameBoard
				successfulMove = flc.move(gb);
			}
			// else, there are two human players
			else {
				// get the next move from the player as a char and int (ex: f3)
				std::cout << "Choose your next move: ";
				std::cin >> playerY;
				std::cin >> playerX;
				// attempt to perform the move and store the returned value in successfulMove
				successfulMove = gb.place('X', (playerX - 1), (int)(playerY - 97));
			}
		}
		// if the move was successful...
		if (successfulMove){
			// toggle playerOneToggle, so it'll be the other player's turn
			playerOneToggle = !playerOneToggle;
			std::cout << std::endl;
			// print the board after the execution of the last move
			gb.printBoard();
		}
		// else the move was illegal
		else {
			// inform the user that the move was illegal
			std::cout << "Not a legal move!" << std::endl;
		}
	}

	// if the loop ended because of a cat's game, inform user
	if (gb.getCatsGame()){
		std::cout << "Cat's game!" << std::endl;
	}

	// otherwise the game ended with four in a row
	// if player one is toggled then the computer or Player 2 won on the previous turn
	else if (playerOneToggle){
		// if there was an AI...
		if (computerPlayer){
			// the AI won
			std::cout << "Computer wins!" << std::endl;
		}
		// else
		else {
			// Player 2 won
			std::cout << "Player 2 wins!" << std::endl;
		}
	}

	// else, it is the computer's or Player 2's "turn" and the player won on previous turn
	else {
		if (computerPlayer){
			std::cout << "You win!" << std::endl;
		}
		else {
			std::cout << "Player 1 wins!" << std::endl;
		}
	}
	std::cout << "Thanks for playing!" << std::endl;

	// empty choice to prevent program from stopping
	int doorStop;
	std::cin >> doorStop;
	return 0;
}