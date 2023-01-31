//
// Created by caleb on 1/31/2023.
//

/**
 * What is this?
 *   This is a collection of classes that I have written to use for assignments in the
 *   CISP400 Spring 2023 semester.
 *
 * Usage:
 *   1. #include <stdexcept>
 *   2. If multiple files allowed...
 *     2a. If C-style includes are allowed...
 *       2i. #include "cbrock.cpp"
 *     2b. Copy & Paste everything in the namespace (or only classes required) to the top of the desired source file.
 * */

#include <stdexcept>

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
}