// Specification B2 - Source File Header
// pig.cpp
// Caleb Brock, CISP 400
// 1/29/2023

#include <iostream>
#include <string>
#include <limits>

using namespace std;

class PigGame {
	private:
		int playerScore;
		int cpuScore;
		int round;
		bool aborted;
	public:
		PigGame() {
			this->playerScore = 0;
			this->cpuScore = 0;
			this->round = 1;
			this->aborted = false;
		};
		~PigGame();
		int getMenuSelection();
		int playGame();
		void playRound();
		int getPlayerScore() { return this->playerScore; };
		int getCpuScore() { return this->cpuScore; };
		int getRound() { return this->round; };
		bool isAborted() { return this->aborted; };

};

void ProgramGreeting();
string getStudentName();
int D6();
int RandomNumber(int, int);


int main() {
	ProgramGreeting();

	// Specification C1 - Fixed Seed
	unsigned int seed = 0;
	srand(seed);

	string studentName = getStudentName();

	// Specification B3 - hiScore on Heap
	int* hiScore = new int();
	*hiScore = 0;


	PigGame* currentGame;
	while (true) {
		currentGame = new PigGame();
		int hiGameScore = currentGame->playGame();
		if (hiGameScore > *hiScore) {
			*hiScore = hiGameScore;
		}

		if (currentGame->isAborted())
			break;
	}


	// Specification B4 - Display hiScore	
	cout << "The high score was " << *hiScore << endl
		<< "Thanks for playing" << endl;
	delete hiScore;
	return 0;
}

// Specification C4 - ProgramGreeting
void ProgramGreeting() {
	cout << "Program by Caleb Brock" << endl;
}

// Specification C2 - Student Name
string getStudentName() {
	cout << "Please enter your name" << endl;
	string name;
	getline(cin, name);
	return name;
}

// Specification C3 - Numeric Menu
int PigGame::getMenuSelection() {
	int input = 0;
	while (true) {
		cout << "Please select an option:" << endl
			<< "1. Roll" << endl
			<< "2. Hold" << endl
			<< "3. Quit" << endl;

		// Specification A4 - Input Validation
		if (!(cin >> input)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input, please try again!" << endl;
			continue;
		} else if (input < 1 || input > 3) {
			cout << "Invalid input, please try again!" << endl;
			continue;
		} else {
			return input;
		}
	}
}

int PigGame::playGame() {
	while(this->playerScore < 100 && this->cpuScore < 100 && !this->isAborted()) {
		playRound();
		this->round++;
	}

	if (this->isAborted()) {
		cout << "The game was aborted." << endl;
		return 0;
	} else if (this->playerScore >= 100) {
		cout << "You won the game with " << this->playerScore << " points!" << endl;
		return this->playerScore;
	} else {
		cout << "The computer won the game with " << this->cpuScore << " points!" << endl;
		return this->cpuScore;
	}
}

void PigGame::playRound() {
	int playerRoundPoints = 0,
		cpuRoundPoints = 0,
		menuSelection = 0;

	// Handle player's turn
	do {
		menuSelection = this->getMenuSelection();
		if (menuSelection == 3) {
			this->aborted = true;
			break;
		} else if (menuSelection == 2) {
			// Player holds
			this->playerScore += playerRoundPoints;
			break;
		}

		int roll = RandomNumber(1, 6);
		cout << "You rolled a " << roll << "!" << endl;
		if (roll == 1) {
			cout << "You bust!" << endl;
			break;
		} else {
			playerRoundPoints += roll;
			cout << "Your new total: " << playerRoundPoints << endl;
		}
	} while(menuSelection == 1);

	int cpuRoll;
	// Handle Computer's turn
	do {
		cpuRoll = RandomNumber(1, 6);
		cout << "The CPU rolled a " << cpuRoll << "!" << endl;
		if (cpuRoll == 1) {
			cout << "The CPU bust!" << endl;
			cpuRoundPoints = 0;
			break;
		} else {
			cpuRoundPoints += cpuRoll;
			cout << "The CPU's new total: " << cpuRoundPoints << endl;
		}
	} while(cpuRoll > 3);
	this->cpuScore += cpuRoundPoints;

	// Specification B1 - Display Turn Stats
	cout << "Round " << this->getRound() << " is over!" << endl
		<< "Player Score: " << this->getPlayerScore() << endl
		<< "CPU Score: " << this->getCpuScore() << endl << endl;
}

// Specification A1 - D6() function
int D6() {
	// Returns a random number between 0 and 5 and adjusts up by 1
	return rand() % 6 + 1;
}

// Specification A2 - RandomNumber() function
int RandomNumber(int lo, int hi) {
	// Specification A3 - Protect RandomNumber() input
	if (lo < 1 || hi < lo || hi > 100)
		return -1;

	return (rand() % (hi - lo) + lo);
}