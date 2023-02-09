// hokeemon.cpp
// Caleb Brock, CISP 400
// April 23, 2023

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std;

#include "../cbrock.cpp"

using namespace cbrock;

// Specification C2 - Creature class
// Specification B2 - Virtual class creature
class Creature {
private:
	virtual ostream& display(ostream& out) const { return out; };
protected:
	int boredom;
	int hunger;
public:
	virtual void PassTime() {};
	virtual void Play() {};
	virtual void Feed() {};
	virtual int getBoredom() const { return 0; };
	virtual int getHunger() const { return 0; };
	// Specification C4 - Overload <<
	friend ostream& operator<<(ostream& out, const Creature& creature) {
		return creature.display(out);
	};
};

// Specification B1 - Child class
class Deez : public Creature {
private:
	ostream& display(ostream& out) const override {
		out << "Creature boredom: " << getBoredom() << endl
			<< "Creature hunger: " << getHunger() << endl;
		return out;
	};
public:
	Deez() {
		srand((unsigned int) time(nullptr));
		boredom = rand() % 6;
		hunger = rand() % 6;
	};
	// Specification C1 - PassTime();
	void PassTime() override {
		boredom++;
		hunger--;
	};
	void Play() override {
		boredom -= rand() % 5 + 4;
	};
	void Feed() override {
		hunger += rand() % 4 + 3;
	};
	int getBoredom() const override {
		return this->boredom;
	};
	int getHunger() const override {
		return this->hunger;
	};
};

char GetMenuInput(istream&, ArrayList<char>*);
void UnitTest();

int main(int argc, char** argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "test")) {
			cout << "Testing!" << endl;
			UnitTest();
			return 0;
		}
	}

	char tmpValid[5] = {'L', 'P', 'F', 'Q'};
	ArrayList<char>* validChars = new ArrayList<char>(tmpValid, 5);

	Creature* c = new Deez();

	char input;
	do {
		cout << "Menu: " << endl
			<< "<L>isten" << endl
			<< "<P>lay" << endl
			<< "<F>eed" << endl
			<< "<Q>uit" << endl;
		input = GetMenuInput(cin, validChars);

		switch (input) {
			case 'L':
				cout << *c;
				break;
			case 'P':
				c->Play();
				break;
			case 'F':
				c->Feed();
				break;
			case 'Q':
			default:
				break;
		}

		c->PassTime();
	} while (input != 'Q');

	delete validChars;
	return 0;
}

void UnitTest() {
	Date::ComponentTest();
	ArrayList<void*>::ComponentTest();
	Assert::analyze();
}

// Specification C3 - Validate Input
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