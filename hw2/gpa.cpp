// gpa.cpp
// Caleb Brock, CISP 400
// February 19, 2023

#include <iostream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

void ProgramGreeting();
void CopyArr(const int*, int*, int);

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

char Grade2Lttr(int);
int GetMenuSelection();
void AddGrade(DynamicArray*);
void PrintScores(DynamicArray*);
int ComputeGpa(DynamicArray*);
void TestDynamicArray();

// Specification B4 - Highlight failing grades
class FancyText {
	public:
		void PrintLine(const char* output, const char* color) {
			cout << "\033[" << color << "m" << output << "\033[0m" << endl;
		}
};

class Date {
public:
	Date();
	~Date();
};

int main() {
	ProgramGreeting();
	TestDynamicArray();
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

void AcceptScores() {

}

void TestDynamicArray() {
	DynamicArray* arr = new DynamicArray();
	arr->push(10);
	arr->push(18);
	arr->push(3);
	arr->push(234);
	arr->push(17);
	cout << "Size of arr is 5: " << (arr->getSize() == 5 ? "true" : "\033[31mfalse\033[0m") << endl;
	arr->remove(3);
	cout << "Size of arr after removal is 4: " << (arr->getSize() == 4 ? "true" : "\033[31mfalse\033[0m") << endl;
	cout << "Value at index 2 is 3: " << (arr->at(2) == 3 ? "true" : "\033[31mfalse\033[0m") << endl;
	cout << "Value at index 3 is 17: " << (arr->at(3) == 17 ? "true" : "\033[31mfalse\033[0m") << endl;
	arr->remove(3);
	cout << "Size of arr after removal is 3: " << (arr->getSize() == 3 ? "true" : "\033[31mfalse\033[0m") << endl;
	arr->remove(0);
	cout << "Value at index 0 is 18 after removing index 0: " << (arr->at(0) == 18 ? "true" : "\033[31mfalse\033[0m") << endl;

}

// Specification C2 - Print scores
void PrintScores(DynamicArray* arr) {
	FancyText fancyText;
	for (int i = 0; i < arr->getSize(); i++) {
		stringstream ss;
		ss << "Score " << i << ": " << arr->at(i);

		if (arr->at(i) < 70) {
			fancyText.PrintLine(ss.str().c_str(), "31");
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
		<< "February XX, 2023" << endl;
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