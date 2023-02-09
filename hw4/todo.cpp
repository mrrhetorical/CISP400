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
		setDate(other.getDate());
		setId(other.getId());
		return *this;
	};

	bool operator==(const Todo& other) const {
		return getContent() == other.getContent() && getDate() == other.getDate() && getId() == other.getId();
	};

	// Specification C3 - Test TODO's
	static void ComponentTest() {
		Todo x("Test");
		Todo y("something");
		x = y;

		Assert::assertTrue(x == y, "Setting Todo x to y means x and y are equal");

		x.setContent("New item");
		Assert::assertEquals<string>("New item", x.getContent(), "Changing the content of Todo x works");

		Assert::assertEquals<string>("something", y.getContent(), "Changing the content of Todo x does not change the content of y");
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
	Assert::analyze();
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