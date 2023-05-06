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

// Specification C2 - Creature class
// Specification B2 - Virtual class creature
class Creature {
private:
	virtual ostream& display(ostream& out) const = 0;
protected:
	// Specification A1 - Critter name
	string name;
	int boredom;
	int hunger;
public:
	virtual void PassTime() = 0;
	virtual void Play() = 0;
	virtual void Feed() = 0;
	virtual const string& getName() const = 0;
	virtual int getBoredom() const = 0;
	virtual void setBoredom(int) = 0;
	virtual int getHunger() const = 0;
	virtual void setHunger(int) = 0;
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

// This class needn't implement the methods from Creature as Deez already implements them. This is simply overriding.
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

	// Specification A4 - Write a Lambda
	auto isStarving = [](Creature& c) { return c.getHunger() < 0; };
	auto isBored = [](Creature& c) { return c.getBoredom() > 20; };

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

		if (isStarving(*c)) {
			cout << "Your Hokeemon died of starvation!" << endl;
			input = 'Q';
		}
		if (isBored(*c)) {
			cout << "Your Hokeemon died of boredom!" << endl;
			input = 'Q';
		}
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