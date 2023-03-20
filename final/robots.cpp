#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

#include "../cbrock.cpp"

using namespace cbrock;

class Tuple {
private:
	int x, y;
public:
	Tuple() {
		setX(0);
		setY(0);
	};
	Tuple(int x, int y) {
		setX(x);
		setY(y);
	};
	void setX(int value) {
		this->x = value;
	};
	int getX() const {
		return this->x;
	};
	void setY(int value) {
		this->y = value;
	}
	int getY() const {
		return this->y;
	}
	Tuple& operator=(const Tuple& other) {
		setX(other.getX());
		setY(other.getY());
		return *this;
	};
};

enum Square {
	NOTHING = 0x0,
	WALL = 0x1,
	BATTERY = 0x2
};

enum GeneMask {
	GENES = 0xff,
	GENE_NORTH = 0xc0,
	GENE_SOUTH = 0x30,
	GENE_EAST = 0xc,
	GENE_WEST = 0x3,
	INSTRUCTION = 0xf000
};

// This is the map for the robots
class Map {
private:
	Square** squares;
	Tuple dimensions;
public:
	Map(const Tuple& dimensions) {
		this->dimensions = dimensions;

		this->squares = new Square*[this->dimensions.getY()];
		for (int i = 0; i < this->dimensions.getY(); i++) {
			this->squares[i] = new Square[this->dimensions.getX()];
		}
	};
	~Map() {
		for (int i = 0; i < this->dimensions.getY(); i++) {
			delete this->squares[i];
		}
		delete [] this->squares;
	};
	Square peek(const Tuple& coords) const {
		if (coords.getX() < 0 || coords.getX() >= this->dimensions.getX() || coords.getY() < 0 || coords.getY() > this->dimensions.getY()) {
			throw invalid_argument("Coordinates can't be out of bounds of the map!");
		}
		return this->squares[coords.getY()][coords.getX()];
	}
};

class Robot {
private:

	/**
	 * See - GeneMask enum
	 *
	 * Sensor Flags:
	 * State ---- N  S  E  W
	 * 0000  0000 00 00 00 00
	 *
	 * 0000
	 *
	 * Sensor Flag Chart
	 * 00 - Nothing
	 * 01 - Wall
	 * 10 - Battery
	 *
	 * Sensor Output Flag Chart
	 * 0000 - 0 - Move random direction
	 * 0001 - 1 - Move north
	 * 0010 - 2 - Move south
	 * 0011 - 3 - Move east
	 * 0100 - 4 - Move west
	 *
	 * Bitwise OR these to set the flags
	 * Bitwise AND these to compare the flags
	 *
	 * Output is stored in the first 12 bits of the byte
	 * To extract: flag >> 12 to shift 12 right & get operation
	 * */
	unsigned int* genes;
	int power;

	// Returns true if the instruction segment (last 8 bits) of the gene match
	bool compareGene(unsigned int a, unsigned int b) {
		// This compares just the last 4 bits of the byte (11111111 = 0xff)
		return (a & GeneMask::GENES) == (b & GeneMask::GENES);
	};

	unsigned int getInstructionFromGene(unsigned int gene) {
		// Bit-shifting a 32-bit unsigned integer by 0x1b units leaves only the last 4 bits
		return gene >> 0x1b;
	}
public:
	Robot() {
		genes = new unsigned int[16];
		for (int i = 0; i < 16; i++) {
			for (int k = 0; k < 4; k++) {
				unsigned int sensorGeneValue = rand() % 0x3;
				sensorGeneValue <<= 0x2 * k; // Shift to 1-3

			}
			//todo: generate random genes
		}
	};

	// Gets the instruction part from a gene
	unsigned int getInstruction(unsigned int sensors) {
		for (int i = 0; i < 16; i++) {
			if (compareGene(sensors, genes[i])) {
				return getInstructionFromGene(genes[i]);
			}
		}
		return 0;
	};

};

void UnitTest();

int main(int argc, char** argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "test")) {
			cout << "Testing!" << endl;
			UnitTest();
			return 0;
		}
	}

	Map* map = new Map(Tuple(10, 10));

	return 0;
}

void UnitTest() {
	ArrayList<void*>::ComponentTest();
	Date::ComponentTest();
	Assert::analyze();
}