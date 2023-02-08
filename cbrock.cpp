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

	class FancyText {
	public:
		static const int ASCII_RED = 31;
		static const int ASCII_GREEN = 32;
		static ostream& PrintLine(ostream& stream, const char* output, const char* color) {
			stream << "\033[" << color << "m" << output << "\033[0m" << endl;
			return ostream;
		};
		static ostream& Print(ostream& stream, const char* output, const char* color) {
			stream << "\033[" << color << "m" << output << "\033[0m";
			return stream;
		};
		static ostream& PrintBool(ostream& stream, bool value) {
			deliminate(stream, value ? "true" : "false", value ? to_string(FancyText::ASCII_GREEN).c_str() : to_string(FancyText::ASCII_RED).c_str());
			return stream&;
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

			cout << "Created array has correct size: ";
			FancyText::deliminateBoolean(cout, a->length() == 4);

			a->remove(2);
			cout << endl << "Deleting element decrements size: ";
			FancyText::deliminateBoolean(cout, a->length() == 3);

			cout << endl << "New truncated array is as expected: ";
			FancyText::deliminateBoolean(cout, a->at(0) == 10 && a->at(1) == 13 && a->at(2) == 256);

			a->put(1, 999);
			cout << endl << "Setting the second element changes the value as expected: ";
			FancyText::deliminateBoolean(cout, a->at(1) == 999);
			cout << endl;


		};
	};

	class Date {
	private:
		tm date;
	public:
		Date() {
			time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
			this->date = *localtime(&time);
		}
		Date(int month, int day, int year) {
			this->SetDate(month, day, year);
		}
		void PrintDate(ostream& stream) const {
			stream << setfill('0') << setw(2) << (date.tm_mon + 1)
				   << "/" << setfill('0') << setw(2) << date.tm_mday
				   << "/" << setfill('0') << setw(4) << (date.tm_year + 1900);
		}
		void SetDate(int month, int day, int year) {
			// tm_mon is months since January so have to subtract 1
			date.tm_mon = month - 1;
			date.tm_mday = day;
			// tm_year is years since 1900 so have to subtract 1900
			date.tm_year = year - 1900;
		}
		static void ComponentTest() {
			cout << "Testing Date class" << endl;
			Date* other = new Date();
			other->SetDate(4, 20, 2023);
			stringstream testStream;
			other->PrintDate(testStream);

			bool setProperly = other->date.tm_mon == 3 && other->date.tm_year == 123 && other->date.tm_mday == 20;
			cout << "Date values set properly: ";
			FancyText::deliminate(cout, setProperly ? "true" : "false", setProperly ? to_string(FancyText::ASCII_GREEN).c_str() : to_string(FancyText::ASCII_RED).c_str());
			cout << endl;

			bool outputMatches = testStream.str() == "04/20/2023";
			cout << "Date output matches: ";
			FancyText::deliminate(cout, outputMatches ? "true" : "false", outputMatches ? to_string(FancyText::ASCII_GREEN).c_str() : to_string(FancyText::ASCII_RED).c_str());
			cout << endl;
			delete other;
		}
		Date& operator=(const Date& other) {
			this->date = other.date;
			return *this;
		};
		bool operator==(const Date& other) {
			return date.tm_mday == other.date.tm_mday && date.tm_year == other.date.tm_year && date.tm_mon == other.date.tm_mon;
		};
		friend istream& operator>>(istream& in, Date& d) {
			int month, day, year;
			char garbage;
			in >> month >> garbage >> day >> garbage >> year;
			d.SetDate(month, day, year);
			return in;
		};
		friend ostream& operator<<(ostream& out, const Date& d) {
			out << setfill('0') << setw(2) << (d.date.tm_mon + 1)
				<< "/" << setfill('0') << setw(2) << d.date.tm_mday
				<< "/" << setfill('0') << setw(4) << (d.date.tm_year + 1900);
			return out;
		};
	};
}