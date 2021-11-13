#include <iostream>
#include "Checkers.h"
#include "Man.h"
using namespace std;

int main() {
	try {
		string filename;
		cout << "Enter input filename: ";
		cin >> filename;

		Checkers game(filename);

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

		size_t lastindex = filename.find_last_of(".");
		filename = filename.substr(0, lastindex) +"-output.txt";
		game.save(filename);

		cout << "Result was saved. File: " << filename << endl;
	}
	catch (exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
