// todo.cpp
// Caleb Brock, CISP 400
// April 2, 2023

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include "../cbrock.cpp"

using namespace std;

using namespace cbrock;

class Todo {
private:
	string content;
	Date date;
	int id;
public:
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

	// Specification C3 - Test TODO's
	static void ComponentTest() {
		Todo a("Test");
		Todo b("something");
		a = b;

		cout << "A value: " << a.getContent() << endl;

		cout << "Done testing!" << endl;


		//todo: finish testing
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
		inStream.clear();
		inStream.ignore(numeric_limits<streamsize>::max(), '\n');
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
					userInput = userInput.substr(1, userInput.length() - 1);
					userInput = trimWhitespace(userInput);
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

					Todo* tmp = todoList->at(index);
					todoList->remove(index);
					delete tmp;

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
	return str.substr(first, str.length() - first);
}

void UnitTest() {
	Todo::ComponentTest();
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