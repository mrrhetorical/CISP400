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

namespace cbrock {
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

		const int length() {
			return this->size;
		}

		// Get the value at a point in the array
		const int at(int index) {
			if (index < 0 || index >= length()) {
				throw std::invalid_argument("Index out of bounds!");
			}
			return *(arr + index);
		}

		const bool contains(T a) {
			for (int i = 0; i < length(); i++) {
				if (at(i) == a)
					return true;
			}
			return false;
		}

		// Get the pointer at the index in the array
		const T* ptrAt(int index) {
			if (index < 0 || index >= length()) {
				throw std::invalid_argument("Index out of bounds!");
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
			int *tmp = arr;
			arr = newArr;
			delete[] tmp;
		}

		// Removes an element at the array
		void remove(int index) {
			if (index < 0 || index >= length()) {
				throw std::invalid_argument("Index out of bounds!");
			}
			T *newArr = new T[size - 1];
			CopyArr(arr, newArr, index);
			CopyArr(arr + index + 1, newArr + index, length() - index);
			T *pTmp = arr;
			arr = newArr;
			size--;
			delete[] pTmp;
		}
	};

	class Date {
	private:
		tm date;
	public:
		Date() {
			time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			this->date = *localtime(&time);
		}
		Date(int month, int day, int year) {
			this->SetDate(month, day, year);
		}
		void PrintDate(std::ostream& stream) {
			stream << "Today's date is "
				   << std::setfill('0') << std::setw(2) << (date.tm_mon + 1)
				   << "/" << std::setfill('0') << std::setw(2) << date.tm_mday
				   << "/" << std::setfill('0') << std::setw(4) << (date.tm_year + 1900);
		}
		void SetDate(int month, int day, int year) {
			// tm_mon is months since January so have to subtract 1
			date.tm_mon = month - 1;
			date.tm_mday = day;
			// tm_year is years since 1900 so have to subtract 1900
			date.tm_year = year - 1900;
		}
	};
}