// BlackJack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "blackjack.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Blackjack game;
	bool running = true;
	cout << "Initial draw:" << endl;
	do {
		game.draw();
		game.monteCarlo();
		game.update();
	} while (game.isRunning());
	return 0;
}

