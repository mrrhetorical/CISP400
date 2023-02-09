// gpa.cpp
// Caleb Brock, CISP 400
// February 19, 2023

/**
 * !!Important!!
 * Run this program with the parameter 'test' to run Unit Tests
 * ex: './gpa.exe test'
 * */

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <chrono>
#include <cstring>
#include <stdexcept>

using namespace std;

namespace cbrock {

	enum AsciiColor {
		CLEAR = 0,
		BOLD = 1,
		UNDERLINE = 4,
		BLACK = 30,
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		MAGENTA = 35,
		CYAN = 36,
		WHITE = 37,
	};

	class FancyText {
	private:
		int color;
		void setColor(int value) {
			this->color = value;
		};
	public:
		FancyText(int color) {
			this->setColor(color);
		};
		int getColor() const {
			return this->color;
		};
		friend ostream& operator<<(ostream& out, const FancyText& fancyText) {
			out << "\033[" << to_string(fancyText.getColor()) << "m";
			return out;
		}
	};

	class Assert {
	private:
		static int tests;
		static int failures;
	public:
		template <typename T> static void assertEquals(T expected, T actual, string text = "") {
			tests++;
			if (expected != actual) {
				failures++;
				cout << FancyText(AsciiColor::RED) << "Test failure!" << endl;
				if (!text.empty()) {
					cout << "Failed test: " << endl;
					cout << text << endl;
				}
				cout << "Expected value: " << expected << endl
					 << "Actual value: " << actual << FancyText(AsciiColor::CLEAR) << endl;
			}
		};
		static void assertTrue(bool value, string text = "") {
			assertEquals(true, value, text);
		};
		static void analyze() {
			cout << "Tests ran: " << Assert::tests << endl;
			cout << "Test failures: " << Assert::failures << endl;
		}
	};

	int Assert::tests = 0;
	int Assert::failures = 0;

	// This is a dynamically allocated array class
	template <typename T> class ArrayList {
	private:
		T *arr;
		int size;

		// Copies the contents from src to dest arrays up to len values
		static void CopyArr(const T *src, T *dest, int len) {
			for (int i = 0; i < len; i++) {
				dest[i] = src[i];
			}
		}

	public:
		ArrayList() {
			arr = nullptr;
			size = 0;
		}
		ArrayList(const T* startingArr, int size) {
			arr = new T[size];
			CopyArr(startingArr, arr, size);
			this->size = size;
		}
		~ArrayList() {
			delete[] arr;
		}

		int length() const {
			return this->size;
		}

		bool isEmpty() const {
			return length() == 0;
		}

		// Get the value at a point in the array
		T at(int index) const {
			if (index < 0 || index >= length()) {
				throw invalid_argument("Index out of bounds!");
			}
			return *(arr + index);
		}

		void put(int index, T value) {
			if (index < 0 || index >= length()) {
				throw invalid_argument("Index out of bounds!");
			}

			*(arr + index) = value;
		}

		bool contains(T a) const {
			for (int i = 0; i < length(); i++) {
				if (at(i) == a)
					return true;
			}
			return false;
		}

		// Get the pointer at the index in the array
		const T* ptrAt(int index) {
			if (index < 0 || index >= length()) {
				throw invalid_argument("Index out of bounds!");
			}
			return arr + index;
		}

		// Get the array if you want to for some reason
		const T* getArray() {
			return this->arr;
		}

		// Push a new value to the array
		void push(T value) {
			T *newArr = new T[size + 1];
			// Only copy array if it has any contents
			if (length() > 0) {
				CopyArr(arr, newArr, size);
			}
			newArr[size] = value;
			size++;
			T *tmp = arr;
			arr = newArr;
			delete[] tmp;
		}

		// Removes an element at the array
		void remove(int index) {
			if (index < 0 || index >= length()) {
				throw invalid_argument("Index out of bounds!");
			}
			T *newArr = new T[size - 1];
			CopyArr(arr, newArr, index);
			CopyArr(arr + index + 1, newArr + index, length() - index - 1);

			T *pTmp = arr;
			arr = newArr;
			size--;
			delete[] pTmp;
		}

		static void ComponentTest() {
			ArrayList<int>* a = new ArrayList<int>();
			a->push(10);
			a->push(13);
			a->push(-1);
			a->push(256);

			Assert::assertEquals(4, a->length(), "Ensure created ArrayList has correct size");

			a->remove(2);
			Assert::assertEquals(3, a->length(), "Ensure deleting elements from ArrayList decreases the size");

			Assert::assertTrue(a->at(0) == 10 && a->at(1) == 13 && a->at(2) == 256, "Ensure new array list is as expected");

			a->put(1, 999);
			Assert::assertEquals(999, a->at(1), "Check that setting the second element of the ArrayList changes the value");
		};
	};

	// Specification A1 - Date class
	class Date {
	private:
		int day, month, year;
		void setMonth(int value) {
			this->month = value;
		};
		void setDay(int value) {
			this->day = value;
		};
		void setYear(int value) {
			this->year = value;
		};
	public:
		Date() {
			time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
			tm t = *localtime(&time);
			this->SetDate(t.tm_mon + 1, t.tm_mday, t.tm_year + 1900);
		};
		Date(int month, int day, int year) {
			this->SetDate(month, day, year);
		};
		// Specification A2 - External date initialization
		void SetDate(int month, int day, int year) {
			this->month = month;
			this->day = day;
			this->year = year;
		};
		int getMonth() const {
			return this->month;
		};
		int getDay() const {
			return this->day;
		};
		int getYear() const {
			return this->year;
		};
		Date& operator=(const Date& other) {
			setMonth(other.getMonth());
			setDay(other.getDay());
			setYear(other.getYear());
			return *this;
		};
		bool operator==(const Date& other) const {
			return getMonth() == other.getMonth() && getDay() == other.getDay() && getYear() == other.getYear();
		};
		friend istream& operator>>(istream& in, Date& date) {
			int m, d, y;
			char garbage;
			in >> m >> garbage >> d >> garbage >> y;
			date.SetDate(m, d, y);
			return in;
		};
		friend ostream& operator<<(ostream& out, const Date& date) {
			out << setfill('0') << setw(2) << date.getMonth()
				<< "/" << setfill('0') << setw(2) << date.getDay()
				<< "/" << setfill('0') << setw(4) << date.getYear();
			return out;
		};
		// Specification A3 - Component Test Method in Date
		static void ComponentTest() {
			Date* other = new Date();
			other->SetDate(4, 20, 2023);
			stringstream testStream;
			testStream << *other;

			Assert::assertTrue(other->month == 4 && other->year == 2023 && other->day == 20, "Check that date values are set properly");

			Assert::assertTrue(testStream.str() == "04/20/2023", "Check that the date output matches the format \"MM/DD/YYYY\"");
			delete other;
		}
	};
}

using namespace cbrock;

void ProgramGreeting();
char Grade2Lttr(int);
int GetMenuSelection();
void AddGrade(ArrayList<int>*);
void PrintScores(ArrayList<int>*);
int ComputeGpa(ArrayList<int>*);
void TestGrades();
void UnitTest();

int main(int argc, char** argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "test")) {
			cout << "Testing!" << endl;
			UnitTest();
			return 0;
		}
	}

	ProgramGreeting();

	// Specification B1 - Dynamic Array
	ArrayList<int>* arr = new ArrayList<int>();

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
	ArrayList<void*>::ComponentTest();
	Date::ComponentTest();
	TestGrades();
	Assert::analyze();
}

void TestGrades() {
	Assert::assertEquals('A', Grade2Lttr(100), "100% should be A");
	Assert::assertEquals('A', Grade2Lttr(92), "92% should be A");
	Assert::assertEquals('B', Grade2Lttr(86), "86% should be B");
	Assert::assertEquals('C', Grade2Lttr(72), "72% should be C");
	Assert::assertEquals('D', Grade2Lttr(67), "67% should be D");
	Assert::assertEquals('F', Grade2Lttr(42), "42% should be F");
	Assert::assertEquals('F', Grade2Lttr(0), "0% should be F");
	Assert::assertEquals('X', Grade2Lttr(101), "101% is out of bounds (X)");
	Assert::assertEquals('X', Grade2Lttr(-1), "-1% is out of bounds (X)");
}

// Specification C2 - Print scores
void PrintScores(ArrayList<int>* arr) {
	for (int i = 0; i < arr->length(); i++) {
		stringstream ss;
		ss << "Score " << i << ": " << arr->at(i);

		// Specification B4 - Highlight Failing Grades
		if (arr->at(i) < 70) {
			cout << FancyText(AsciiColor::RED);
		}
		cout << ss.str() << FancyText(AsciiColor::CLEAR) << endl;
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
	cout << *date << endl;
	delete date;
}

// Specification B2 - Add elements
void AddGrade(ArrayList<int>* array) {
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
int ComputeGpa(ArrayList<int>* array) {
	int total = 0;
	for (int i = 0; i < array->length(); i++) {
		total += array->at(i);
	}
	return (total / array->length());
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
