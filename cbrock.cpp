//
// Created by caleb on 1/31/2023.
//

/**
 * What is this?
 *   This is a collection of classes that I have written to use for assignments in the
 *   CISP400 Spring 2023 semester.
 *
 * Usage:
 *   1. Use all include statements
 *   2. If multiple files allowed...
 *     2a. If C-style includes are allowed...
 *       2i. #include "cbrock.cpp"
 *     2b. Copy & Paste everything in the namespace (or only classes required) to the top of the desired source file.
 * */

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <chrono>

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

	class ColoredBool {
	private:
		bool value;
	public:
		ColoredBool(bool value) {
			this->value = value;
		};
		friend ostream& operator<<(ostream& out, const ColoredBool& coloredBool) {
			if (coloredBool.value) {
				out << FancyText(AsciiColor::GREEN) << "true";
			} else {
				out << FancyText(AsciiColor::RED) << "true";
			}
			out << FancyText(AsciiColor::CLEAR);
			return out;
		};
	};

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
			cout << "Testing ArrayList class..." << endl;

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
		bool operator==(const Date& other) {
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