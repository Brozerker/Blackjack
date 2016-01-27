#include <random>
#include <vector>
#include <iostream>
using namespace std;
#include <string>
#include <sstream>
#include <time.h>

struct Card {
	int value = 0;
	char character = 'X';
};

struct Player {
	bool dealer;
	bool myTurn;
	vector<Card> hand;
	int score = 0;
};

class Blackjack {
	bool running = true;
	Player dealer, user;

public:
	Blackjack() {
		srand(time(0)); 
		dealer.dealer = true;	user.dealer = false;
		dealer.myTurn = false;	user.myTurn = true;
		hit(dealer);	hit(dealer);
		hit(user);		hit(user);
	}

	void draw() {
		if (!user.myTurn) {
			cout << "\nDealers hand:\t(total " << dealer.score << ")" << endl;
			for (int i = 0; i < dealer.hand.size(); ++i) {
				if (dealer.hand[i].value > 1 && dealer.hand[i].value < 10)
					cout << (int)dealer.hand[i].character << "  ";
				else
					cout << dealer.hand[i].character << "  ";
			}
		}
		else {
			cout << "\nDealers hand:\t(total #)" << endl;
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

	void monteCarlo() {
		if (user.myTurn) {
			cout << "\nMonte Carlo simulation running....." << endl;
			cout << "Playing 1000 games to determine the probility of winning if you stand" << endl;
			float standWin = 0;
			float hitWin = 0;
			for (int i = 0; i < 1000; ++i) {
				if (simulateHit(user.hand))		hitWin++;
				if (simulateStand())			standWin++;
			}
			cout << "Monte Carlo simulation stand resulted in " << standWin << " wins and " << 1000 - standWin << " losses" << endl;
			cout << "Monte Carlo simulation hit resulted in " << hitWin << " wins and " << 1000 - hitWin << " losses" << endl;
			cout << "The probability of winning if you stand is " << (standWin / 1000) * 100 << "%" << endl;
			cout << "The probability of winning if you hit is " << (hitWin / 1000) * 100 << "%" << endl;
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
					endGame();
					return;
				}
			}
			if (choice == 's')
				stand(user);
		}

		if (dealer.myTurn) {
			if (dealer.score >= 17) {
				stand(dealer);
				endGame();
			}
			if (dealer.score < 17)
				hit(dealer);
			if (dealer.score > 21)
				endGame();
		}
	}

	int aceDraw(Player & who) {
		if (who.dealer) {
			if (who.score < 11)
				return 11;
			else
				return 1;
		}
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
		int choice = (rand() % 12) + 1;
		Card newCard;
		if (choice == 1) {
			newCard.value = aceDraw(who);
			newCard.character = 'A';
		} 
		else if (choice > 9) {
			newCard.value = 10;
			newCard.character = 'F';
		} 
		else {
			newCard.value = choice;
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

	void endGame() {
		cout << "\nYour score: " << user.score << endl;
		cout << "Dealers score: " << dealer.score << endl;
		if ((dealer.score > user.score && dealer.score < 22) || user.score > 21)
			cout << "You Lose!" << endl;
		else if (dealer.score < user.score || dealer.score > 21)
			cout << "You Win!" << endl;			
		running = false;
	}

	bool isRunning() {
		return running;
	}

	bool simulateHit(vector<Card> hand) {
		vector<Card> other = dealer.hand;
		int score = 0;
		for (int i = 0; i < hand.size(); ++i) {
			score += hand[i].value;
		}
		hand.push_back(simulationHit(score));
		if (score > 21)
			return false;
		if (score == 21)
			return true;
		return simulateDealer(other, score);
	}
	Card simulationHit(int & score) {
		int choice = (rand() % 12) + 1;
		Card newCard;
		if (choice == 1) {
			if (score < 11) 	newCard.value = 11;
			else				newCard.value = 1;
		}
		else if (choice > 9) 	newCard.value = 10;
		else					newCard.value = choice;
		score += newCard.value;
		return newCard;
	}
	bool simulateStand() {
		vector<Card> other = dealer.hand;
		return simulateDealer(other, user.score);
	}
	bool simulateDealer(vector<Card> hand, int score) {
		int dealerScore = 0;
		for (int i = 0; i < hand.size(); ++i) {
			dealerScore += hand[i].value;
		}
		if (dealerScore == 21)		return false;
		if (dealerScore > 21)		return true;
		if (dealerScore > 17)		return (score > dealerScore);
	
		hand.push_back(simulationHit(dealerScore));
		return simulateDealer(hand, score);
	}
};