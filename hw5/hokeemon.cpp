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
	// Specification A1 - Critter name
	string name;
	int boredom;
	int hunger;
public:
	virtual void PassTime() {};
	virtual void Play() {};
	virtual void Feed() {};
	virtual const string& getName() const { return ""; };
	virtual int getBoredom() const { return 0; };
	virtual void setBoredom(int) {};
	virtual int getHunger() const { return 0; };
	virtual void setHunger(int) {};
	// Specification C4 - Overload <<
	friend ostream& operator<<(ostream& out, const Creature& creature) {
		return creature.display(out);
	};
	// Specification B4 - Overload +
	// Also overloaded in the child class
	virtual Creature& operator+(const Creature& b) const {
		return const_cast<Creature &>(b);
	};
};

// Specification B1 - Child class
class Deez : public Creature {
private:
	ostream& display(ostream& out) const override {
		out << "Creature " << getName() << "'s boredom: " << getBoredom() << endl
			<< "Creature " << getName() << "'s hunger: " << getHunger() << endl;
		return out;
	};
public:
	Deez(const string& name, unsigned int seed = time(nullptr)) {
		this->name = name;
		srand(seed);
		boredom = rand() % 6;
		hunger = rand() % 6;
	};
	// Specification A2 - Copy Constructor
	Deez(const Creature& other) {
		this->name = other.getName();
		this->boredom = other.getBoredom();
		this->hunger = other.getHunger();
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
	const string& getName() const override {
		return this->name;
	};
	int getBoredom() const override {
		return this->boredom;
	};
	void setBoredom(int value) override {
		this->boredom = value;
	};
	int getHunger() const override {
		return this->hunger;
	};
	void setHunger(int value) override {
		this->hunger = value;
	};
	Creature& operator+(const Creature& other) const override {
		Creature* c = new Deez(*this);
		c->setHunger(this->getHunger() + other.getHunger());
		c->setBoredom(this->getBoredom() + other.getBoredom());
		return *c;
	};
	static void ComponentTest() {
		// Seed random for tests
		Creature* c = new Deez("TestName", 0);

		Assert::assertEquals<string>("TestName", c->getName(), "Setting name for creature works");
		Assert::assertEquals<int>(3, c->getHunger(), "Hunger is set as expected");
		Assert::assertEquals<int>(2, c->getBoredom(), "Boredom is set as expected");

		c->Play();
		Assert::assertEquals<int>(-5, c->getBoredom(), "Playing with creature decreases boredom");

		c->Feed();
		Assert::assertEquals<int>(7, c->getHunger(), "Feeding creature decreases hunger");

		c->PassTime();
		Assert::assertEquals<int>(6, c->getHunger(), "Hunger decreases from passing time");
		Assert::assertEquals<int>(-4, c->getBoredom(), "Boredom increases from passing time");

		Creature* b = new Deez(*c);
		Assert::assertEquals<string>("TestName", b->getName(), "Name of copied creature is the same");
		Assert::assertEquals<int>(6, b->getHunger(), "Hunger of copied creature is the same");
		Assert::assertEquals<int>(-4, b->getBoredom(), "Boredom of copied creature is the same!");

		c->PassTime();
		Assert::assertEquals<int>(6, b->getHunger(), "Passing time on original creature doesn't affect copied creature");
		Assert::assertEquals<int>(-4, b->getBoredom(), "Passing time on original creature doesn't affect copied creature");

		Creature* a = &((*b) + (*c));
		Assert::assertEquals<string>("TestName", a->getName(), "Name of added creature was set properly");
		Assert::assertEquals<int>(11, a->getHunger(), "New hunger is added between the first two");
		Assert::assertEquals<int>(-7, a->getBoredom(), "New boredom is added between the first two");


		delete a;
		delete b;
		delete c;
	};

};

// Specification A3 - Second child class
class Foo : public Deez {
public:
	void PassTime() override {
		boredom += 2;
		hunger -= 3;
	};
	void Play() override {
		boredom -= rand() % 6 + 3;
	};
	void Feed() override {
		hunger += rand() % 7 + 4;
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

	string creatureName;
	cout << "Please enter a name for your creature:" << endl;
	do {
		getline(cin, creatureName);

		if (creatureName.empty() || creatureName.length() < 3) {
			cout << "Please enter a different name for your creature!" << endl;
		}
	} while (creatureName.empty() || creatureName.length() < 3);

	char tmpValid[5] = {'L', 'P', 'F', 'Q'};
	ArrayList<char>* validChars = new ArrayList<char>(tmpValid, 5);

	Creature* c = new Deez(creatureName);

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
	Deez::ComponentTest();
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