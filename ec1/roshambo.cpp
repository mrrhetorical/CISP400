// roshambo.cpp
// Caleb Brock, CISP 400
// May 18, 2023

#include <iostream>
#include <chrono>
#include <cstring>
#include <regex>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;

#include "../cbrock.cpp"

using namespace cbrock;

struct PersistentGameData {
	int playerWins = 0,
		computerWins = 0,
		handsPlayed = 0;
};

enum RockPaperScissors {
	ROCK = 0,
	PAPER = 1,
	SCISSORS = 2
};

void UnitTest();

void GameLoop();
void PlayRound(PersistentGameData&);
void DisplayGameData(PersistentGameData&);
int compare(RockPaperScissors, RockPaperScissors);

int main(int argc, char** argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "test")) {
			cout << "Testing!" << endl;
			UnitTest();
			return 0;
		}
	}

	GameLoop();

	return 0;
}

void GameLoop() {
	PersistentGameData data;
	while (true) {
		string input;
		cout << "What would you like to do?\n1. [P]lay\n2. [V]iew scores\n3. [Q]uit" << endl;
		getline(cin, input);
		if (regex_match(input, regex("P|(play)|1", regex_constants::icase))) {
			PlayRound(data);
		} else if (regex_match(input, regex("V|(view)|2", regex_constants::icase))) {
			DisplayGameData(data);
		} else if (regex_match(input, regex("Q|(quit)|3", regex_constants::icase))) {
			DisplayGameData(data);
			break;
		} else {
			cout << "Invalid input, please try again!" << endl;
		}
	}
}

void PlayRound(PersistentGameData& data) {
	RockPaperScissors playerChoice;
	while (true) {
		string input;
		cout << "Pick your choice between rock, paper, or scissors:" << endl;
		getline(cin, input);
		if (regex_match(input, regex("(rock)|r", regex_constants::icase))) {
			playerChoice = RockPaperScissors::ROCK;
		} else if (regex_match(input, regex("(paper)|p", regex_constants::icase))) {
			playerChoice = RockPaperScissors::PAPER;
		} else if (regex_match(input, regex("(scissors)|s", regex_constants::icase))) {
			playerChoice = RockPaperScissors::SCISSORS;
		} else {
			cout << "Invalid input, please try again!" << endl;
			continue;
		}
		break;
	}

	srand(time(nullptr));
	RockPaperScissors cpuChoice = static_cast<RockPaperScissors>(rand() % 3);

	cout << "The computer picked " << (cpuChoice == RockPaperScissors::ROCK ? "rock" : cpuChoice == RockPaperScissors::PAPER ? "paper" : "scissors") << "!" << endl;

	data.handsPlayed++;
	int result = compare(playerChoice, cpuChoice);
	if (result > 0) {
		data.playerWins++;
		cout << "You won this round!" << endl;
	} else if (result < 0) {
		data.computerWins++;
		cout << "You lost this round!" << endl;
	} else {
		cout << "You drew this round!" << endl;
	}
}

void DisplayGameData(PersistentGameData& data) {
	cout << "You have played a total of " << data.handsPlayed << " hands." << endl
		<< "You won " << data.playerWins << " of those hands," << endl
		<< "the computer won " << data.computerWins << " of those hands," << endl
		<< "and you drew " << (data.handsPlayed - data.playerWins - data.computerWins) << " of those hands!" << endl;
}

/**
 * Return positive number if a > b, or a negative if a < b, or 0 if equal
 * */
int compare(RockPaperScissors a, RockPaperScissors b) {
	if (a == b)
		return 0;

	if (a == RockPaperScissors::ROCK) {
		return b == RockPaperScissors::SCISSORS ? 1 : -1;
	} else if (a == RockPaperScissors::PAPER) {
		return b == RockPaperScissors::ROCK ? 1 : - 1;
	}

	return b == RockPaperScissors::PAPER ? 1 : -1;
}

void UnitTest() {
	Date::ComponentTest();
	ArrayList<void*>::ComponentTest();
	Assert::analyze();
}