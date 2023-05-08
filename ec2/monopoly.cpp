// monopoly.cpp
// Caleb Brock, CISP 400
// May 18, 2023

#include <iostream>
#include <stdexcept>
#include <limits>
#include <fstream>
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

	class RandNo {
	private:
		static RandNo* instance;
		int seed;
		RandNo(int seed = 0) {
			this->seed = seed;
			srand(this->seed);
		};
		// Returns a random float between 0.0 and 1.0
		float rand01() const {
			return (static_cast<float>(rand()) / static_cast<float>(numeric_limits<float>::max()));
		}
	public:
		static const RandNo getInstance() {
			if (instance == nullptr) {
				instance = new RandNo(time(0));
			}

			return *instance;
		};
		int getRandomNumber(int min, int max) const {
			if (min > max) {
				throw invalid_argument("The minimum value must be below the maximum value!");
			}
			return ((rand() % (max - min)) + min);
		};
		float getRandomNumber(float min, float max) const {
			if (min > max) {
				throw invalid_argument("The minimum value must be below the maximum value!");
			}
			return (rand01() * (min - max) + min);
		}
	};

	RandNo* RandNo::instance = nullptr;
}

using namespace cbrock;

int rollDie();
template<typename T> void shuffle(ArrayList<T>*);

class Card;

enum LocationType {
	REGULAR, UTILITY, RAILROAD, CHANCE, COMMUNITY_CHEST
};

struct Location {
	int timesLandedOn = 0;
	LocationType locationType = LocationType::REGULAR;
};

class Board {
private:
	int currentPosition;
	Location map[40];
	const int jailSquare = 10;
	const int goToJailSquare = 30;
	ArrayList<Card*>* chanceDeck;
	ArrayList<Card*>* communityChestDeck;
public:
	Board(ArrayList<Card*>* chanceCards, ArrayList<Card*>* communityChestCards) {
		currentPosition = 0;
		map[2].locationType = LocationType::COMMUNITY_CHEST;
		map[5].locationType = LocationType::RAILROAD;
		map[7].locationType = LocationType::CHANCE;
		map[12].locationType = LocationType::UTILITY;
		map[15].locationType = LocationType::RAILROAD;
		map[17].locationType = LocationType::COMMUNITY_CHEST;
		map[22].locationType = LocationType::CHANCE;
		map[25].locationType = LocationType::RAILROAD;
		map[28].locationType = LocationType::UTILITY;
		map[33].locationType = LocationType::COMMUNITY_CHEST;
		map[35].locationType = LocationType::RAILROAD;
		map[36].locationType = LocationType::CHANCE;

		this->chanceDeck = chanceCards;
		this->communityChestDeck = communityChestCards;
	};
	int getCurrentPosition() const {
		return currentPosition;
	};
	const Location* getMap() const {
		return map;
	};
	void advance(int squares) {
		currentPosition += squares;
		// Wrap around if passing around go
		currentPosition %= 40;
		move(currentPosition);
		if (currentPosition == goToJailSquare) {
			currentPosition = jailSquare;
		}
	};
	void move(int square) {
		currentPosition = square;
		map[currentPosition].timesLandedOn++;
	};
	void playCards();
	void playTurn() {
		int a = rollDie(), b = rollDie();
		advance(a + b);
		playCards();
		if (a == b) {
			a = rollDie(), b = rollDie();
			advance(a + b);
			playCards();
			if (a == b) {
				a = rollDie(), b = rollDie();
				if (a != b) {
					advance(a + b);
					playCards();
				} else {
					move(jailSquare);
				}
			}
		}
	};
	void outputData(ostream& out) {
		for (int i = 0; i < 40; i++) {
			out << i << "," << map[i].timesLandedOn << endl;
		}
	}
};

// Virtual class for
class Card {
public:
	virtual ~Card() = default;
	virtual int getAmountToAdvance(const Board& board) {
		return 0;
	};
};

// This is a card that represents just advancing a certain amount of squares. Example: go back 3 squares.
class AdvanceCard : public Card {
private:
	int squaresToAdvance;
public:
	explicit AdvanceCard(int squaresToAdvance) {
		this->squaresToAdvance = squaresToAdvance;
	};
	int getAmountToAdvance(const Board& board) override {
		return squaresToAdvance;
	}
};

// This is a card that represents going to a particular square. Example: go to jail, boardwalk, etc.
class GoToCard : public Card {
private:
	int target;
public:
	explicit GoToCard(int target) {
		this->target = target;
	};
	int getAmountToAdvance(const Board& board) override {
		int current = board.getCurrentPosition();
		if (current > target) {
			return 40 - (current - target);
		} else {
			return target - current;
		}
	};
};

// This is a card that represent cards like "go to the nearest railroad/utility"
class GoToTypeCard : public Card {
private:
	LocationType locationType;
public:
	explicit GoToTypeCard(LocationType locationType) {
		this->locationType = locationType;
	};
	int getAmountToAdvance(const Board& board) override {
		const Location* map = board.getMap();
		int moved = 0;
		int position = board.getCurrentPosition();
		++position %= 40;
		while (map[position].locationType != locationType) {
			moved++;
			++position %= 40;
			if (moved >= 40) {
				cerr << "Couldn't find location of type!" << endl;
				break;
			}
		}

		return moved;
	};
};


int main() {
	cout << "Welcome to the monopoly sim!" << endl;

	ArrayList<Card*>* chanceDeck = new ArrayList<Card*>();

	chanceDeck->push(new GoToCard(0)); // Advance to Go
	chanceDeck->push(new GoToCard(24)); // Advance to illinois ave
	chanceDeck->push(new GoToCard(11)); // Advance to st charles place
	chanceDeck->push(new GoToTypeCard(LocationType::UTILITY)); // Advance to the nearest utility
	chanceDeck->push(new GoToTypeCard(LocationType::RAILROAD)); // Advance to the nearest railroad
	chanceDeck->push(new AdvanceCard(-3)); // Go back 3 spaces
	chanceDeck->push(new GoToCard(10)); // Go To jail
	chanceDeck->push(new GoToCard(5)); // Go to reading railroad
	chanceDeck->push(new GoToCard(39)); // Go to boardwalk
	for (int i = chanceDeck->length(); i < 16; i++) {
		chanceDeck->push(new Card());
	}

	shuffle<Card*>(chanceDeck);

	ArrayList<Card*>* communityChest = new ArrayList<Card*>();
	communityChest->push(new GoToCard(0)); // Advance to Go
	communityChest->push(new GoToCard(10)); // Go to jail

	for (int i = communityChest->length(); i < 17; i++) {
		communityChest->push(new Card());
	}

	shuffle<Card*>(communityChest);

	Board* board = new Board(chanceDeck, communityChest);

	cout << "How many turns do you want to simulate?" << endl;
	int turns;
	while (!(cin >> turns) || turns <= 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter a proper value:" << endl;
	}

	for (int i = 0; i < turns; i++) {
		board->playTurn();
	}

	board->outputData(cout);
	ofstream results("results.csv");
	if (!results.is_open()) {
		cerr << "Couldn't open results.csv file!" << endl;
		return 1;
	}

	board->outputData(results);
	results.close();

	cout << "Finished writing to file!" << endl;

	for (int i = 0; i < chanceDeck->length(); i++) {
		delete chanceDeck->at(i);
	}
	for (int i = 0; i < communityChest->length(); i++) {
		delete communityChest->at(i);
	}

	delete board;

	return 0;
}

int rollDie() {
	return (rand() % 6) + 1;
}

void Board::playCards() {

	ArrayList<Card*>* deck;

	switch (map[currentPosition].locationType) {
		case LocationType::CHANCE:
			deck = chanceDeck;
			break;
		case LocationType::COMMUNITY_CHEST:
			deck = communityChestDeck;
			break;
		default:
			return;
	}

	Card* card = deck->at(0);
	deck->remove(0);
	deck->push(card);

	int toAdvance = card->getAmountToAdvance(*this);
	advance(toAdvance);
}

// This shuffle's an ArrayList's values. The values should be sufficiently random.
template<typename T> void shuffle(ArrayList<T>* arr) {
	srand(time(nullptr));

	for (int i = arr->length() - 1; i > 0; i--) {
		int j = rand() % (i + 1);

		// Swap
		T temp = arr->at(i);
		arr->put(i, arr->at(j));
		arr->put(j, temp);
	}
}