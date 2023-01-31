// gpa.cpp
// Caleb Brock, CISP 400
// February 19, 2023

/**
 * !!Important!!
 * Run this program with the parameter 'test' to run Unit Tests
 * ex: './gpa.exe test'
 * */

// I really didn't want to have to remember these
#define ASCII_RED "31"
#define ASCII_GREEN "32"

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>
#include <cstring>

using namespace std;

class Date;
class DynamicArray;
class FancyText;

void ProgramGreeting();
void CopyArr(const int*, int*, int);
char Grade2Lttr(int);
int GetMenuSelection();
void AddGrade(DynamicArray*);
void PrintScores(DynamicArray*);
int ComputeGpa(DynamicArray*);
void TestDynamicArray();
void TestDate();
void TestGrades();
void UnitTest();

// Specification B4 - Highlight failing grades
class FancyText {
public:
	static void PrintLine(const char* output, const char* color) {
		cout << "\033[" << color << "m" << output << "\033[0m" << endl;
	}
	static void deliminate(ostream& stream, const char* output, const char* color) {
		stream << "\033[" << color << "m" << output << "\033[0m";
	}
};

// Specification A1 - Date class
class Date {
private:
	tm date;
public:
	Date() {
		time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
		this->date = *localtime(&time);
	}
	void PrintDate(ostream& stream) {
		stream << "Today's date is "
			<< setfill('0') << setw(2) << (date.tm_mon + 1)
			<< "/" << setfill('0') << setw(2) << date.tm_mday
			<< "/" << setfill('0') << setw(4) << (date.tm_year + 1900) << endl;
	}
	// Specification A2 - External Date Initialization
	void SetDate(int month, int day, int year) {
		// tm_mon is months since January so have to subtract 1
		date.tm_mon = month - 1;
		date.tm_mday = day;
		// tm_year is years since 1900 so have to subtract 1900
		date.tm_year = year - 1900;
	}
	// Specification A3 - Component Test Method in Date
	static void CompTest() {
		Date* other = new Date();
		other->SetDate(4, 20, 2023);
		stringstream testStream;
		other->PrintDate(testStream);

		bool setProperly = other->date.tm_mon == 3 && other->date.tm_year == 123 && other->date.tm_mday == 20;
		cout << "Date values set properly: ";
		FancyText::deliminate(cout, setProperly ? "true" : "false", setProperly ? ASCII_GREEN : ASCII_RED);
		cout << endl;

		bool outputMatches = testStream.str() == "Today's date is 04/20/2023\n";
		cout << "Date output matches: ";
		FancyText::deliminate(cout, outputMatches ? "true" : "false", outputMatches ? ASCII_GREEN : ASCII_RED);
		cout << endl;
		delete other;
	}
};

// Specification B1 - Dynamic Array
// This class stores a dynamic array and handles scaling it when new elements are pushed.
// This class doesn't handle removing an element from the array.
class DynamicArray {
private:
	int* arr;
	int size;
public:
	DynamicArray() {
		arr = nullptr;
		size = 0;
	}
	~DynamicArray() {
		delete [] arr;
	}
	int getSize() {
		return this->size;
	}
	// Get the value at a point in the array
	int at(int index) {
		return *(arr + index);
	}
	// Get the pointer at the index in the array
	int* ptrAt(int index) {
		return arr + index;
	}
	// Push a new value to the array
	void push(int value) {
		int* newArr = new int[size + 1];
		// Only copy array if it has any contents
		if (getSize() > 0) {
			CopyArr(arr, newArr, size);
		}
		newArr[size] = value;
		size++;
		int* tmp = arr;
		arr = newArr;
		delete [] tmp;
	}
	// Removes an element at the array
	void remove(int index) {
		if (index < 0 || index >= getSize())
			return;
		int* newArr = new int[size - 1];
		CopyArr(arr, newArr, index);
		CopyArr(arr + index + 1, newArr + index, getSize() - index);
		int* pTmp = arr;
		arr = newArr;
		size--;
		delete [] pTmp;
	}
	// Get the array if you want to for some reason
	const int* getArray() {
		return this->arr;
	}
};


int main(int argc, char** argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "test")) {
			cout << "Testing!" << endl;
			UnitTest();
			return 0;
		}
	}

	ProgramGreeting();

	DynamicArray* arr = new DynamicArray();

	int menuSelection;
	do {
		menuSelection = GetMenuSelection();
		switch (menuSelection) {
			case 1:
				AddGrade(arr);
				break;
			case 2:
				PrintScores(arr);
				break;
			case 3: {
				int average = ComputeGpa(arr);
				cout << "The average grade is " << average << "%!" << endl;
				break;
			}
			default:
			case 4:
				break;
		}

	} while (menuSelection != 4);

	return 0;
}

// Specification A4 - Unit Test
void UnitTest() {
	TestDynamicArray();
	TestDate();
	TestGrades();
}

void TestDynamicArray() {
	DynamicArray* arr = new DynamicArray();
	arr->push(10);
	arr->push(18);
	arr->push(3);
	arr->push(234);
	arr->push(17);

	cout << "Size of arr is 5: ";
	FancyText::deliminate(cout, (arr->getSize() == 5 ? "true" : "false"), (arr->getSize() == 5 ? ASCII_GREEN : ASCII_RED));
	cout << endl;

	arr->remove(3);
	cout << "Size of arr after removal is 4: ";
	FancyText::deliminate(cout, (arr->getSize() == 4 ? "true" : "false"), (arr->getSize() == 4 ? ASCII_GREEN : ASCII_RED));
	cout << endl;

	cout << "Value at index 2 is 3: ";
	FancyText::deliminate(cout, (arr->at(2) == 3 ? "true" : "false"), (arr->at(2) == 3 ? ASCII_GREEN : ASCII_RED));
	cout << endl;

	cout << "Value at index 3 is 17: ";
	FancyText::deliminate(cout, (arr->at(3) == 17 ? "true" : "false"), (arr->at(3) == 17 ? ASCII_GREEN : ASCII_RED));
	cout << endl;

	arr->remove(3);
	cout << "Size of arr after removal is 3: ";
	FancyText::deliminate(cout, (arr->getSize() == 3 ? "true" : "false"), (arr->getSize() == 3 ? ASCII_GREEN : ASCII_RED));
	cout << endl;

	arr->remove(0);
	cout << "Value at index 0 is 18 after removing index 0: ";
	FancyText::deliminate(cout, (arr->at(0) == 18 ? "true" : "false"), (arr->at(0) == 18 ? ASCII_GREEN : ASCII_RED));
	cout << endl;
}

void TestDate() {
	Date::CompTest();
}

void TestGrades() {
	cout << "Grade 100 is A: ";
	FancyText::deliminate(cout, (Grade2Lttr(100) == 'A' ? "true" : "false"), Grade2Lttr(100) == 'A' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade 92 is A: ";
	FancyText::deliminate(cout, (Grade2Lttr(92) == 'A' ? "true" : "false"), Grade2Lttr(92) == 'A' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade 86 is B: ";
	FancyText::deliminate(cout, (Grade2Lttr(86) == 'B' ? "true" : "false"), Grade2Lttr(86) == 'B' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade 72 is C: ";
	FancyText::deliminate(cout, (Grade2Lttr(72) == 'C' ? "true" : "false"), Grade2Lttr(72) == 'C' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade 67 is D: ";
	FancyText::deliminate(cout, (Grade2Lttr(67) == 'D' ? "true" : "false"), Grade2Lttr(67) == 'D' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade 42 is F: ";
	FancyText::deliminate(cout, (Grade2Lttr(42) == 'F' ? "true" : "false"), Grade2Lttr(42) == 'F' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade 0 is F: ";
	FancyText::deliminate(cout, (Grade2Lttr(0) == 'F' ? "true" : "false"), Grade2Lttr(0) == 'F' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade 101 is out of bounds (X): ";
	FancyText::deliminate(cout, (Grade2Lttr(101) == 'X' ? "true" : "false"), Grade2Lttr(101) == 'X' ? ASCII_GREEN : ASCII_RED);
	cout << endl;

	cout << "Grade -1 is out of bounds (X): ";
	FancyText::deliminate(cout, (Grade2Lttr(-1) == 'X' ? "true" : "false"), Grade2Lttr(-1) == 'X' ? ASCII_GREEN : ASCII_RED);
	cout << endl;
}

// Specification C2 - Print scores
void PrintScores(DynamicArray* arr) {
	for (int i = 0; i < arr->getSize(); i++) {
		stringstream ss;
		ss << "Score " << i << ": " << arr->at(i);

		if (arr->at(i) < 70) {
			FancyText::PrintLine(ss.str().c_str(), "31");
		} else {
			cout << ss.str() << endl;
		}
	}
}

int GetMenuSelection() {
	int input = 0;
	do {
		cout << "Please select an option:" << endl
			<< "1. Add Grade" << endl
			<< "2. Display All Grades" << endl
			<< "3. Process All Grades" << endl
			<< "4. Quit Program" << endl;

		// Specification B3 - Menu Input validation
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
		<< "February 19, 2023" << endl;

	Date* date = new Date();
	date->PrintDate(cout);
	delete date;
}

// Specification B2 - Add elements
void AddGrade(DynamicArray* array) {
	int grade;
	cout << "Please enter a grade:" << endl;
	while (!(cin >> grade)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input, please try again" << endl;
	}

	array->push(grade);
	cout << "Grade added!" << endl;
}

// Specification C4 - Compute Gpa
int ComputeGpa(DynamicArray* array) {
	int total = 0;
	for (int i = 0; i < array->getSize(); i++) {
		total += array->at(i);
	}
	return (total / array->getSize());
}

// Copies the contents from src to dest arrays up to len values
void CopyArr(const int* src, int* dest, int len) {
	for (int i = 0; i < len; i++) {
		dest[i] = src[i];
	}
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
