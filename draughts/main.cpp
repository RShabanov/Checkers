#include <iostream>
#include "Checkers.h"
#include "Man.h"
using namespace std;

// find out what color makes first move
Color firstColor();

int main() {
	try {
		constexpr unsigned int movesNumber = 2;

		string filename;
		cout << "Enter input filename: ";
		cin >> filename;

		Checkers game(filename);

		game.runNMoves(movesNumber, firstColor()); // play only N moves
		//game.run(firstColor()); // play the whole game

		// if after game.runNMoves the game is not over
		// we have to tell it
		if (game.state() == GameState::STILL_PLAYING) {
			cout << "Game is not over" << endl;;
		}

		// find index where file extension starts
		size_t lastindex = filename.find_last_of(".");
		// remove this extension and add "-output.txt"
		filename = filename.substr(0, lastindex) +"-output.txt";
		game.save(filename);

		cout << endl << "Result was saved. File: " << filename << endl;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
	}

	return 0;
}

Color firstColor() {
	char firstColor;

	while (true) {
		cout << "First move (B or W): _\b";
		cin >> firstColor;
		firstColor = toupper(firstColor);

		if (strchr("BW", firstColor)) break;

		system("cls");
		cout << "Please, pay attention! Let's try again" << endl;;
	}

	return Color(firstColor == 'W' ? Color::WHITE : Color::BLACK);
}
