#include <iostream>
#include "Checkers.h"
using namespace std;

int main() {
	//Checkers ch("input.txt");
	
	char firstMove;

	while (true) {
		cout << "First move (B or W): _\b";
		cin >> firstMove;
		firstMove = toupper(firstMove);

		if (strchr("BW", firstMove)) break;

		system("cls");
		cout << "Please, pay attention! Let's try again" << endl;;
	}

	return 0;
}
