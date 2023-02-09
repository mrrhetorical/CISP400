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

	// Specification B1 - Date class
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
		// Specification B2 - Component Test method in Date
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
			   << setw(12) << item.getDateAddedToInventory() << endl;

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

		Assert::assertEquals<int>(0, item->getWholesaleCost(), "Check default wholesale cost");
		Assert::assertEquals<string>("", item->getItemId(), "Check default item id");
		Assert::assertEquals<int>(0, item->getRetailCost(), "Check default retail cost");

		item->setWholesaleCost(9.99);

		Assert::assertEquals<float>(9.99, item->getWholesaleCost(), "Changing wholesale cost works");
		Assert::assertEquals<float>(19.98, item->getRetailCost(), "Changing wholesale cost affects the retail cost");

		delete item;
		item = new Item("12345", 12, 2);

		Assert::assertEquals<int>(2, item->getWholesaleCost(), "Check parameterized constructor set wholesale cost");
		Assert::assertEquals<string>("12345", item->getItemId(), "Check parameterized constructor set item id");
		Assert::assertEquals<int>(4, item->getRetailCost(), "Check parameterized constructor set retail cost");
		Assert::assertEquals<int>(12, item->getQuantity(), "Check parameterized constructor set quantity");

		item->setQuantity(9);

		Assert::assertEquals<int>(9, item->getQuantity(), "Check that changing quantity works");

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
	ArrayList<void*>::ComponentTest();
	Date::ComponentTest();
	Item::ComponentTest();
	Assert::analyze();
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

// Specification A1 - Edit Inventory
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