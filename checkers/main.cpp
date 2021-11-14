#include <iostream>
#include "GameBoard.h"
using namespace std;

int main() {
	GameBoard board{ (State()) };
	
	cout << board << endl;
	
	
	return 0;
}