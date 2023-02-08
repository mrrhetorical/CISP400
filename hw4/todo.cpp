// todo.cpp
// Caleb Brock, CISP 400
// April 2, 2023

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

namespace cbrock {

	class FancyText {
	public:
		static const int ASCII_RED = 31;
		static const int ASCII_GREEN = 32;
		static void PrintLine(ostream& stream, const char* output, const char* color) {
			stream << "\033[" << color << "m" << output << "\033[0m" << endl;
		};
		static void deliminate(ostream& stream, const char* output, const char* color) {
			stream << "\033[" << color << "m" << output << "\033[0m";
		};
		static void deliminateBoolean(ostream& stream, bool value) {
			deliminate(stream, value ? "true" : "false", value ? to_string(FancyText::ASCII_GREEN).c_str() : to_string(FancyText::ASCII_RED).c_str());
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

using namespace cbrock;

class Todo {
private:
	string content;
	Date date;
	int id;
public:
	// Specification A4 - Overload constructor
	// Here I have a couple overloaded constructors. They are useful in a few different scenarios
	Todo() {
		content = "";
		id = -1;
	};

	Todo(const string& content) {
		this->setContent(content);
	};

	Todo(const string& content, int id) {
		this->setContent(content);
		this->setId(id);
	};

	// Specification A1 - Overload Copy Constructor
	Todo(const Todo& todo) {
		*this = todo;
	};

	~Todo() {};

	const string& getContent() const {
		return this->content;
	};

	void setContent(const string& value) {
		this->content = value;
	};

	int getId() const {
		return this->id;
	};

	void setId(int value) {
		this->id = value;
	};

	const Date& getDate() const {
		return this->date;
	}

	void setDate(const Date& value) {
		this->date = value;
	};

	void print(ostream& out) {
		out << setw(10) << getId() << "|"
			<< setw(16) << getDate() << "|"
			<< getContent();
	};

	// Specification C1 - Overload <<
	friend ostream& operator<<(ostream& out, const Todo& todoList) {
		out << todoList.getId() << "|" << todoList.getContent() << "|" << todoList.getDate();
		return out;
	};

	// Specification C2 - Overload >>
	friend istream& operator>>(istream& in, Todo& todoList) {
		char garbage;
		in >> todoList.id;
		in >> garbage;
		getline(in, todoList.content, '|');
		in >> todoList.date;
		return in;
	};

	// Specification A2 - Overload Assignment Operator
	Todo& operator=(const Todo& other) {
		setContent(other.getContent());
		return *this;
	};

	bool operator==(const Todo& other) {
		return getContent() == other.getContent() && date == other.getDate() && id == other.getId();
	};

	// Specification C3 - Test TODO's
	static void ComponentTest() {
		cout << "Testing Todo class..." << endl;

		Todo a("Test");
		Todo b("something");
		a = b;

		cout << "Setting a to b makes a and b equal: ";
		FancyText::deliminateBoolean(cout, a == b);
		cout << endl;

		a.setContent("New item");
		cout << "Changing content of a changes content of a: ";
		FancyText::deliminateBoolean(cout, !strcmp("New item", a.getContent().c_str()));
		cout << endl;

		cout << "Changing content of a does not change content of b: ";
		FancyText::deliminateBoolean(cout, !strcmp("something", b.getContent().c_str()));
		cout << endl;
	};
};

void ProgramGreeting();
bool equalsIgnoreCase(const string&, const string&);
string trimWhitespace(const string&);
int getNewId(const ArrayList<Todo*>*);
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

	// Specification C4 - TODO array
	ArrayList<Todo*>* todoList = new ArrayList<Todo*>();

	const string fileName = "todoList.txt";

	// Specification B4 - Persistence
	// These lines load the todo list from the file
	ifstream inStream(fileName);
	if (inStream.is_open()) {
		Todo* todo = new Todo();
		while (inStream >> *todo) {
			todoList->push(todo);
			todo = new Todo();
		}
		delete todo;
	}
	inStream.close();

	cout << "Menu:" << endl
		 << "+ - Adds an item to the todo list" << endl
		 << "- - Removes an item from the todo list at the given index" << endl
		 << "? - Displays the todo list" << endl
		 << "quit - Quits the program" << endl;

	string userInput;
	do {
		getline(cin, userInput);
		userInput = trimWhitespace(userInput);

		if (userInput.length() == 0) {
			cout << "Invalid input, please try again!" << endl;
		} else if (equalsIgnoreCase(userInput, "quit")) {
			break;
		} else {
			char first = userInput.c_str()[0];
			switch (first) {
				// Specification B1 - + Symbol
				case '+': {
					if (userInput.length() < 2) {
						cout << "Added dummy data!" << endl;
						Todo* todo = new Todo();
						todoList->push(todo);
						break;
					}
					userInput = userInput.substr(1, userInput.length() - 1);
					userInput = trimWhitespace(userInput);
					if (userInput.empty()) {
						cout << "No content, adding dummy data!" << endl;
						Todo* todo = new Todo();
						todoList->push(todo);
						break;
					}
					Todo* todo = new Todo(userInput, getNewId(todoList));
					todoList->push(todo);
					cout << "Added to todo list!" << endl;
					break;
				}
				// Specification B2 - ? Symbol
				case '?': {
					cout << "[Todo List]" << endl
						<< "    ID    |   Date Added   |     Content" << setfill (' ') << endl;
					for (int i = 0; i < todoList->length(); i++) {
						todoList->at(i)->print(cout);
						cout << setfill(' ') << endl;
					}
					break;
				}
				// Specification B3 - - Symbol
				case '-': {
					if (userInput.length() < 2) {
						cout << "You need to specify which element you'd like to remove!" << endl;
						break;
					}
					userInput = userInput.substr(1, userInput.length() - 1);
					stringstream userInputStream(userInput);
					int id;
					if (!(userInputStream >> id)) {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Invalid id!" << endl;
						break;
					}

					int index = -1;

					for (int i = 0; i < todoList->length(); i++) {
						if (todoList->at(i)->getId() == id) {
							index = i;
							break;
						}
					}

					if (index == -1) {
						cout << "There's no such todo list item with that id!" << endl;
						break;
					}

					delete todoList->at(index);

					todoList->remove(index);

					cout << "Item removed!" << endl;
					break;
				}
				default:
					cout << "Unrecognized command, please try again!" << endl;
					break;
			}
		}
	} while (true);

	// These lines saves the todo list to the file
	ofstream outStream (fileName, ios::trunc);
	if (todoList->length() != 0) {
		for (int i = 0; i < todoList->length(); i++) {
			outStream << *todoList->at(i) << endl;
		}
	} else {
		outStream.clear();
	}
	outStream.close();

	return 0;
}

bool equalsIgnoreCase(const string& a, const string& b) {
	if (a.length() != b.length())
		return false;

	for (int i = 0; i < a.length(); i++) {
		if (toupper(a.c_str()[i]) != toupper(b.c_str()[i]))
			return false;
	}

	return true;
}

string trimWhitespace(const string& str) {
	unsigned int first = str.find_first_not_of(' ');
	if (first >= str.length()) {
		return "";
	}
	return str.substr(first, str.length() - first);
}

void UnitTest() {
	Todo::ComponentTest();
	Date::ComponentTest();
	ArrayList<void*>::ComponentTest();
}

void ProgramGreeting() {
	// Specification A3 - System Date
	// The default date constructor grabs the system date and will display it here
	Date date;
	cout << "Welcome!" << endl
		 << "Caleb Brock" << endl
		 << date << endl;
}

int getNewId(const ArrayList<Todo*>* todoList) {
	int index = 0;
	for (int i = 0; i < todoList->length(); i++) {
		if (todoList->at(i)->getId() == index) {
			index++;
			i--;
		}
	}
	return index;
}