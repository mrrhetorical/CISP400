// invinq.cpp
// Caleb Brock, CISP 400
// March 12, 2023

/**
 * !!Important!!
 * Run this program with the parameter 'test' to run Unit Tests
 * ex: './inving.exe test'
 * */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>
#include <stdexcept>

using namespace std;

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
			CopyArr(arr + index + 1, newArr + index, length() - index);
			T *pTmp = arr;
			arr = newArr;
			size--;
			delete[] pTmp;
		}
	};

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
			deliminate(cout, value ? "true" : "false", value ? to_string(FancyText::ASCII_GREEN).c_str() : to_string(FancyText::ASCII_RED).c_str());
		};
	};

	// Specification B1 - Date class
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

		// Specification B2 - ComponentTest method in Date
		static void ComponentTest() {
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
	};
}

using namespace cbrock;

class Item {
private:
	string itemId;
	unsigned int quantity;
	float wholesaleCost;
	float retailCost;
	Date addedToInventory;
public:
	Item() {
		setItemId("");
		setQuantity(0);
		setWholesaleCost(0);
		setRetailCost(0);
	};
	Item(const string& itemId, unsigned int quantity, float wholesaleCost) {
		this->setItemId(itemId);
		this->setQuantity(quantity);
		this->setWholesaleCost(wholesaleCost);
		this->addedToInventory.SetDate(9, 9, 1999);
	};
	~Item() {};
	const string& getItemId() const {
		return this->itemId;
	};
	void setItemId(const string& value) {
		this->itemId = value;
	};
	unsigned int getQuantity() const {
		return this->quantity;
	};
	void setQuantity(unsigned int value) {
		this->quantity = value;
	};
	float getWholesaleCost() const {
		return this->wholesaleCost;
	};
	void setWholesaleCost(float value) {
		this->wholesaleCost = value;
		this->setRetailCost(getWholesaleCost() * 2);
	};
	float getRetailCost() const {
		return this->retailCost;
	};
	void setRetailCost(float value) {
		this->retailCost = value;
	};
	const Date& getDateAddedToInventory() const {
		return this->addedToInventory;
	};
	// Specification A2 - Overload operator <<
	friend ostream& operator<<(ostream& stream, const Item& item) {
		ios init(nullptr);
		init.copyfmt(stream);

		stream << setfill(' ') <<  setw(9) << item.getItemId() << "|"
			   << setw(12) << item.getQuantity() << "|"
			   << setw(16) << fixed << setprecision(2) << item.getWholesaleCost() << "|"
			   << setw(15) << item.getRetailCost() << "|"
			   << setw(12);
		item.getDateAddedToInventory().PrintDate(stream);
		stream << endl;

		stream.copyfmt(init);
		return stream;
	};
	// Specification A3 - Overload operator >>
	friend istream& operator>>(istream& stream, Item& item) {
		cout << "Please enter the quantity:" << endl;
		int quantity;
		do {
			if (!(cin >> quantity)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid quantity, please try again!" << endl;
			} else if (quantity < 0) {
				cin.clear();
				cout << "Quantity cannot be negative! Please try again!" << endl;
			} else {
				break;
			}
		} while (true);

		cout << "Please enter the wholesale cost:" << endl;
		float wholesaleCost;
		do {
			if (!(cin >> wholesaleCost)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid wholesale cost, please try again!" << endl;
			} else if (wholesaleCost < 0) {
				cin.clear();
				cout << "Wholesale cost cannot be negative!" << endl;
			} else {
				break;
			}
		} while (true);

		item.setQuantity(quantity);
		item.setWholesaleCost(wholesaleCost);
		return stream;
	};
	static void ComponentTest() {
		Item* item = new Item();

		bool defaultConstructorWorks = item->getWholesaleCost() == 0 && item->getItemId() == "" && item->getRetailCost() == 0;
		cout << "Default constructor works: ";
		FancyText::deliminateBoolean(cout, defaultConstructorWorks);
		cout << endl;

		item->setWholesaleCost(9.99);
		cout << "Changing wholesale cost changes wholesale and retail costs: ";
		// Comparison of floats is weird, setting low tolerance for comparison
		bool costChanges = item->getWholesaleCost() - 9.99 < 0.0000000001 && item->getRetailCost() - 19.98 < 0.0000000001;
		FancyText::deliminateBoolean(cout, costChanges);
		cout << endl;

		delete item;
		item = new Item("12345", 12, 2);
		cout << "Parameterized constructor works: ";
		bool pConstructor = item->getItemId() == "12345" && item->getQuantity() == 12 && item->getWholesaleCost() == 2 && item->getRetailCost() == 4;
		FancyText::deliminateBoolean(cout, pConstructor);
		cout << endl;

		item->setQuantity(9);
		cout << "Changing quantity works" << endl;
		bool quantityChange = item->getQuantity() == 9;
		FancyText::deliminateBoolean(cout, quantityChange);
		cout << endl;

		delete item;
	};
};

void ProgramGreeting();
void UnitTest();
char GetMenuInput(istream& istream, ArrayList<char>*);
void AddItem(ArrayList<Item*>*);
Item* CreateItem();
Item* CreateItem(const char*);
void DeleteItem(ArrayList<Item*>*);
void EditItem(ArrayList<Item*>*);
void DisplayInventory(ArrayList<Item*>*);
bool isAlpha(const char*);

int main(int argc, char** argv) {

	// Specification A4 - UnitTest() method in main()
	if (argc > 1) {
		if (!strcmp(argv[1], "test")) {
			cout << "Testing!" << endl;
			UnitTest();
			return 0;
		}
	}

	ProgramGreeting();

	// Specification C2 - Dynamic Array
	ArrayList<Item*>* inventory = new ArrayList<Item*>();

	// I'm lazy and hate writing out a million comparisons
	char tmpValid[5] = {'A', 'D', 'I', 'E', 'Q'};
	ArrayList<char>* validChars = new ArrayList<char>(tmpValid, 5);

	char input;
	do {
		cout << "Menu:" << endl
			<< "<A>dd item" << endl
			<< "<D>elete item" << endl
			<< "<E>dit Inventory" << endl
			<< "Display <I>nventory" << endl
			<< "<Q>uit" << endl;
		// Get input and convert to upper for ease of comparison.
		input = GetMenuInput(cin, validChars);
		switch(input) {
			case 'A':
				AddItem(inventory);
				break;
			case 'D':
				DeleteItem(inventory);
				break;
			case 'I':
				DisplayInventory(inventory);
				break;
			case 'E':
				EditItem(inventory);
				break;
			case 'Q':
			default:
				break;
		}
	} while (input != 'Q');

	delete validChars;
	return 0;
}

void UnitTest() {
	Date::ComponentTest();
	Item::ComponentTest();
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

void AddItem(ArrayList<Item*>* itemList) {
	Item* item = CreateItem();

	// Specification C3 - Resize Array - add to end
	itemList->push(item);
}

Item* CreateItem(const char* itemId) {
	Item* item = new Item(itemId, 0, 0);
	cin >> *item;
	return item;
}

Item* CreateItem() {
	// Specification B4 - Inventory Entry Input Validation
	// This spec is covered in this function as well as in CreateItem(const char*)
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

	return CreateItem(itemId);
}

void EditItem(ArrayList<Item*>* itemList) {
	cout << "Please enter the item id of an item you'd like to edit:" << endl;
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

	for (int i = 0; i < itemList->length(); i++) {
		if (!strcmp(itemId, itemList->at(i)->getItemId().c_str())) {
			// Item found, so create a new item (with an id already set) and replace with the current item

			Item* replacementItem = CreateItem(itemId);
			Item* old = itemList->at(i);
			itemList->put(i, replacementItem);
			delete old;

			cout << "Item updated!" << endl;

			return;
		}
	}

	cout << "That item does not exist in the inventory!" << endl;


}

void DeleteItem(ArrayList<Item*>* itemList) {
	if (itemList->isEmpty())
		return;

	delete itemList->at(itemList->length() - 1);

	// Specification C4 - Resize Array - subtract from end
	itemList->remove(itemList->length() - 1);
}

void DisplayInventory(ArrayList<Item*>* itemList) {
	cout << setfill(' ') << " Item Id |"
		   << "  Quantity  |"
		   << " Wholesale Cost |"
		   << "  Retail Cost  |"
		   << "    Date" << endl;

	for (int i = 0 ; i < itemList->length(); i++) {
		cout << *itemList->at(i);
	}
}

// Specification C1 - Alpha menu
char GetMenuInput(istream& istream, ArrayList<char>* validChars) {
	char selection;
	do {
		// Specification B3 - Menu Input Validation
		if (!(istream >> selection)) {
			istream.clear();
			istream.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input, please try again!" << endl;
			continue;
		} else {
			selection = (char) toupper(selection);
			if (!validChars->contains(selection)) {
				cout << "Invalid selection, please try again!" << endl;
				continue;
			}
		}
		break;
	} while (true);

	return selection;
}