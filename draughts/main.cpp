#include <iostream>
#include "Checkers.h"
#include "Man.h"
using namespace std;

int main() {
	// TODO: add file via CLI args
	Checkers game("input2.txt");
	
	char firstColor;

	while (true) {
		cout << "First move (B or W): _\b";
		cin >> firstColor;
		firstColor = toupper(firstColor);

		if (strchr("BW", firstColor)) break;

		system("cls");
		cout << "Please, pay attention! Let's try again" << endl;;
	}

	game.run(firstColor == 'W' ? Color::WHITE : Color::BLACK);
	game.save();

	return 0;
}
