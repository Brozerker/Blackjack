#include <random>
#include <vector>
#include <iostream>
using namespace std;
#include <string>
#include <sstream>
#include <time.h>

struct Card {
	int value = 0;
	char character = 'F';
};
struct Player {
	bool myTurn;
	vector<Card> hand;
	int score = 0;
};

class Blackjack {
	bool running = true;
	Player dealer, user;

public:
	Blackjack() {
		srand(clock());
		dealer.myTurn = false;	user.myTurn = true;
		hit(dealer);	hit(dealer);
		hit(user);		hit(user);
	}

	void draw() {
		if (dealer.myTurn) {
			cout << "Dealers hand:\t(total " << dealer.score << ")" << endl;
			for (int i = 0; i < dealer.hand.size(); ++i) {
				if (dealer.hand[i].value > 1 && dealer.hand[i].value < 10)
					cout << (int)dealer.hand[i].character << "  ";
				else
					cout << dealer.hand[i].character << "  ";
			}
		}
		else {
			cout << "Dealers hand:\t(total #)" << endl;
			cout << "#  "; 
			if (dealer.hand[1].value > 1 && dealer.hand[1].value < 10)
				cout << (int)dealer.hand[1].character << "  ";
			else
				cout << dealer.hand[1].character << "  ";
		}
		cout << "\nUsers hand:\t(total " << user.score << ")" << endl;
		for (int i = 0; i < user.hand.size(); ++i) {
			if (user.hand[i].value > 1 && user.hand[i].value < 10)
				cout << (int)user.hand[i].character << "  ";
			else
				cout << user.hand[i].character << "  ";
		}
	}
	
	void update() {
		if (user.myTurn) {
			cout << "\nWhat do you want to do? (H/S)" << endl;
			string input;
			char choice;
			//getline(cin, input);
			cin >> choice;
			if (choice == 'h') {
				hit(user);
				if (user.score > 21) {
					endGame(false);
					return;
				}
			}
			if (choice == 's')
				stand(user);
		}

		if (dealer.myTurn) {
			if (dealer.score >= 17)
				stand(dealer);
			if (dealer.score < 17)
				hit(dealer);
			if (dealer.score > 21)
				endGame(true);
		}
		else {
			if (dealer.score > user.score)
				endGame(false);
			else
				endGame(true);
		}
	}
	

	int aceDraw() {
		int toReturn;
		string in = "";
		cout << "you drew an Ace. Should it be worth 1 or 11 points?" << endl;
		getline(cin, in);
		stringstream myStream(in);
		myStream >> toReturn;
		return toReturn;
	}

	void hit(Player & who) {
		// chance of F = 16/52
		// chance of A = 4/52
		//chance of 2 - 9 = 4/52
		int choice = rand() % 12;
		Card newCard;
		if (choice == 0) {
			newCard.value = aceDraw();
			newCard.character = 'A';
		} 
		else if (choice > 8) {
			newCard.value = 10;
			newCard.character = 'F';
		} 
		else {
			newCard.value = choice + 1;
			newCard.character = (char)newCard.value;
		}
		who.hand.push_back(newCard);
		who.score += newCard.value;
	}

	void stand(Player & who) {
		if (!dealer.myTurn)
			dealer.myTurn = true;
		who.myTurn = false;
	
	}

	void endGame(bool won) {
		draw();
		if (won)
			cout << "You Win!" << endl;
		else
			cout << "You Lose!" << endl;
		running = false;
	}

	bool isRunning() {
		return running;
	}
};