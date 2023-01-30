// gpa.cpp
// Caleb Brock, CISP 400
// Due Date TBD

#include <iostream>
#include <string>
#include <limits>

using namespace std;

class FancyText {
	public:
		void CoutWithColor(const char* output, const char* color) {

		}
};

void ProgramGreeting();
char Grade2Lttr(int);
int GetMenuSelection();

int main() {
	ProgramGreeting();

	int* grades = new int[1];

	return 0;
}

void AcceptScores() {

}

void PrintScores() {

}

int GetMenuSelection() {
	int input = 0;
	do {
		cout << "Please select an option:" << endl
			<< "1. Add Grade" << endl
			<< "2. Display All Grades" << endl
			<< "3. Process All Grades" << endl
			<< "4. Quit Program";

		// Specification A4 - Input Validation
		if (!(cin >> input)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input, please try again!" << endl;
			continue;
		} else if (input < 1 || input > 4) {
			cout << "Invalid input, please try again!" << endl;
			continue;
		} else {
			return input;
		}
	} while (true);
}

// Specification C1 - Program Greeting Function
void ProgramGreeting() {
	cout << "Welcome!" << endl
		<< "Author: Caleb Brock" << endl
		<< "February XX, 2023" << endl;
}

// Specification C3 - Letter Grades
char Grade2Lttr(int grade) {
	if (grade > 100 || grade < 0) {
		return 'X';
	} else if (grade >= 90) {
		return 'A';
	} else if (grade >= 80) {
		return 'B';
	} else if (grade >= 70) {
		return 'C';
	} else if (grade >= 60) {
		return 'D';
	} else {
		return 'F';
	}
}