// invinq.cpp
// Caleb Brock, CISP 400
// March 12, 2023

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <stdexcept>
#include "../cbrock.cpp"  // todo put into thing

using namespace std;

using namespace cbrock;

class Item {
private:
	string itemId;
	unsigned int quantity;
	float wholeSaleCost;
	float retailCost;
	Date addedToInventory;
public:
	Item();
	~Item();




};

void ProgramGreeting();
char GetMenuInput(istream& istream, ArrayList<char>*);
void AddItem();
void DeleteItem();
void EditItem();
void DisplayInventory();
bool isAlpha(const char*);

int main() {
	ProgramGreeting();

	char* tmpValid = new char[]{'A', 'D', 'I', 'E', 'Q'};
	ArrayList<char>* validChars = new ArrayList<char>(tmpValid, 5);
	delete [] tmpValid;

	char input;
	do {
		cout << "Menu:" << endl
			<< "<A>dd item" << endl
			<< "<D>elete item" << endl
			<< "<E>dit Inventory" << endl
			<< "Display <I>nventory" << endl
			<< "<Q>uit" << endl;
		// Get input and convert to upper for ease of comparison.
		input = (char) toupper(GetMenuInput(cin, validChars));
		switch(input) {
			case 'A':
				AddItem();
				break;
			case 'D':

				break;
			case 'I':
				break;
			case 'E':
				break;
			case 'Q':
			default:
				break;
		}
	} while (input != 'Q');

	delete validChars;
	return 0;
}

void ProgramGreeting() {
	cout << "Welcome!" << endl
		<< "Caleb Brock" << endl
		<< "March 12, 2023" << endl;
}

bool isAlpha(const char* cstr) {
	for (int i = 0; i < strlen(cstr); i++) {
		if (!isalpha(cstr[i]))
			return false;
	}
	return true;
}

void AddItem() {
	cout << "Please enter the item id:" << endl;
	char* itemId = new char[16];
	do {
		if (!(cin >> itemId)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid item id, please try again!" << endl;
		} else if (strlen(itemId) != 5 || isAlpha(itemId)) {
			cout << "Invalid item id, please try again!" << endl;
		} else {
			break;
		}
	} while (true);

	cout << "Please enter the quantity:" << endl;
	int quantity = -1;
	do {
		if (!(cin >> quantity)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid quantity, please try again!" << endl;
		} else if (quantity < 0) {
			cout << "Quantity cannot be negative! Please try again!" << endl;
		} else {
			break;
		}
	} while (quantity < 0);

	cout << "Please enter the wholesale cost:" << endl;
	float wholesaleCost = -1;
	do {
		if (!(cin >> wholesaleCost)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid wholesale cost, please try again!" << endl;
		} else if (wholesaleCost < 0) {
			cout << "Wholesale cost cannot be negative!" << endl;
		} else {
			break;
		}
	} while (wholesaleCost < 0);

	Item i =


}

// Specification C1 - Alpha menu
char GetMenuInput(istream& istream, ArrayList<char>* validChars) {
	char selection;
	do {
		if (!(istream >> selection)) {
			istream.clear();
			istream.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input, please try again!" << endl;
			continue;
		} else if (!validChars->contains(selection)) {
			cout << "Invalid selection, please try again!" << endl;
			continue;
		}
		break;
	} while (true);

	return selection;
}