// monopoly.cpp
// Caleb Brock, CISP 400
// May 18, 2023

#include <iostream>
#include <stdexcept>
#include <limits>
#include <fstream>

using namespace std;

#include "../cbrock.cpp"

using namespace cbrock;


// used to cache the amount of times a square is landed upon
/* template <typename K, typename V> class Map {
private:
	struct Bucket {
		K key;
		V value;
		Bucket* next = nullptr;
		Bucket(K key, V value) {
			this->key = key;
			this->value = value;
		};
	};
	Bucket** buckets;
	int size;
	static const int max_size = 32;
	static int hash(K key) {
		return (key * 17) % max_size;
	}
public:
	Map() {
		size = 0;
		buckets = new Bucket*[max_size];
		for (int i = 0; i < max_size; i++) {
			buckets[i] = nullptr;
		}
	};
	V get(K key) {
		int index = hash(key);
		if (buckets[index] != nullptr) {
			if (buckets[index]->key == key) {
				return buckets[index]->value;
			} else {
				Bucket* ptr = buckets[index];
				while (ptr->next != nullptr) {
					ptr = ptr->next;
					if (ptr->key == key) {
						return ptr->value;
					}
				}
			}
		}
		throw invalid_argument("No such key exists in the map!");
	};
	void put(K key, V value) {
		Bucket* bucket = new Bucket(key, value);
		int index = hash(key);
		if (buckets[index] != nullptr) {
			Bucket* ptr = buckets[index];
			while (ptr->next != nullptr) {
				ptr = ptr->next;
			}
			ptr->next = bucket;
		} else {
			buckets[index] = bucket;
		}

		size++;
	};
	int getSize() {
		return size;
	}

};
*/

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

enum CardType {
	MOVE_TO, ADVANCE, MOVE_TO_TYPE
};

class Card {
public:
	virtual ~Card() = default;;
	virtual int getAmountToAdvance(const Board& board) {
		return 0;
	};
};

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
	cout << "Welcome to the monopoly sim" << endl;

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

#ifdef DEBUG
	board->outputData(cout);
#else
	ofstream results("results.csv");
	if (!results.is_open()) {
		cerr << "Couldn't open results.csv file!" << endl;
		return 1;
	}

	board->outputData(results);
	results.close();

	cout << "Finished writing to file!" << endl;
#endif

	delete board;

	for (int i = 0; i < chanceDeck->length(); i++) {
		delete chanceDeck->at(i);
	}
	for (int i = 0; i < communityChest->length(); i++) {
		delete communityChest->at(i);
	}

	return 0;
}

int rollDie() {
	return (rand() % 6) + 1;
}

void Board::playCards() {

	ArrayList<Card*>* deck = nullptr;

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
};

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