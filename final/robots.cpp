/**
 * Todo: Remove debug defines and includes
 */
//#define DEBUG 0

#define BATTERY_POWER_GAINED 10
#define BATTERY_POWER_LOST_PER_MOVE 1

#ifdef DEBUG
#include <bitset>
#endif

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <fstream>

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
	Tuple& operator+(const Tuple& other) const {
		Tuple* tuple = new Tuple(getX() + other.getX(), getY() + other.getY());
		return *tuple;
	};
	Tuple& operator+=(const Tuple& other) {
		Tuple* x = &(*this + other);
		setX(x->getX());
		setY(x->getY());
		delete x;
		return *this;
	}
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

enum GeneInstruction {
	MOVE_NORTH = 0x0,
	MOVE_SOUTH = 0x1,
	MOVE_EAST = 0x2,
	MOVE_WEST = 0x3,
	MOVE_RANDOM = 0x4
};

// This is the map for the robots
class Map {
private:
	Square** squares;
	Tuple dimensions;
	Tuple startSquare;
	void generateSquares() {
		// Note: This is stored upside down (y = 0 is at the top so it must be rendered from end to beginning to show correctly!)
		this->squares = new Square*[this->dimensions.getY()];
		for (int i = 0; i < this->dimensions.getY(); i++) {
			this->squares[i] = new Square[this->dimensions.getX()];
			for (int k = 0; k < this->dimensions.getX(); k++) {
				this->squares[i][k] = Square::NOTHING;
			}
		}
	};
public:
	Map() {};
	Map(const Tuple& dimensions) {
		this->dimensions = dimensions;
		this->generateSquares();
	};
	~Map() {
		for (int i = 0; i < this->dimensions.getY(); i++) {
			delete this->squares[i];
		}
		delete [] this->squares;
	};
	Square peek(const Tuple& coords) const {
		if (!isValidPosition(coords)) {
			cout << "Out of bounds!" << endl;
			cout <<" Coords: (" << coords.getX() << ", " << coords.getY() << ")" << endl;
			// Allow to peek out of bounds, but just return a wall every time.
			return Square::WALL;
		}

		return this->squares[coords.getY()][coords.getX()];
	};
	void setSquare(const Tuple& pos, Square value) {
		if (!isValidPosition(pos))
			throw out_of_range("The coordinates are out of range!");

		this->squares[pos.getY()][pos.getX()] = value;
	}
	bool isValidPosition(const Tuple& coords) const {
		return !(coords.getX() < 0 || coords.getX() >= this->dimensions.getX() || coords.getY() < 0 || coords.getY() > this->dimensions.getY());
	};
	const Tuple& getStartSquare() const {
		return this->startSquare;
	};
	void setStartSquare(const Tuple& value) {
		this->startSquare = value;
	};
	friend ostream& operator<<(ostream& out, const Map& map) {
		for (int y = map.dimensions.getY() - 1; y >= 0; y--) {
			for (int x = 0; x < map.dimensions.getX(); x++) {
				auto getValue = [](Square s){
					switch (s) {
						case Square::BATTERY:
							return 'B';
						case Square::NOTHING:
							return ' ';
						default:
							return '#';
					}
				};
				out << getValue(map.peek(Tuple(x, y)));
			}
			out << endl;
		}
		return out;
	};

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
	Tuple position;
	int powerHarvested;
	int turnsSurvived;


	// Returns true if the instruction segment (last 8 bits) of the gene match
	bool compareGene(unsigned int a, unsigned int b) {
		// This compares just the last 4 bits of the byte (11111111 = 0xff)
		return (a & GeneMask::GENES) == (b & GeneMask::GENES);
	};

	unsigned int getInstructionFromGene(unsigned int gene) {
		// Bit-shifting a 32-bit unsigned integer by 0x1b units leaves only the last 4 bits
		return gene >> 0xc;
	}
	void generateGenes(int count) {
		genes = new unsigned int[count];
		for (int i = 0; i < count; i++) {
			genes[i] = 0;
			for (int k = 0; k < 4; k++) {
				unsigned int sensorGeneValue = rand() % 0x3; // random number between 0-3 (00, 01, 10, 11)
				sensorGeneValue <<= 0x2 * k; // Shift to 1-3
				genes[i] = genes[i] | sensorGeneValue; // Bitwise OR to set new gene state
			}  // This generates the genes
			unsigned int outputGeneValue = rand() % 0x4; // random number between 0-4
			outputGeneValue <<= 0xc; // Shift it 12 bits left
			genes[i] = genes[i] | outputGeneValue;
		}
	}
public:
	Robot(const Tuple& startingPosition, int startingPower = 10, int geneCount = 16) {
		setPower(startingPower);
		setPosition(startingPosition);
		generateGenes(geneCount);
	};
	int getPower() const {
		return this->power;
	};
	void setPower(int value) {
		this->power = value;
	};
	int getPowerHarvested() const {
		return this->powerHarvested;
	};
	int getTurnsSurvived() const {
		return this->turnsSurvived;
	};
	const Tuple& getPosition() const {
		return this->position;
	};
	void setPosition(const Tuple& value) {
		this->position = value;
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
	unsigned int useSensor(const Map& map) {
		// Check north
		Square north = map.peek(getPosition() + Tuple(0, 1)),
			south = map.peek(getPosition() + Tuple(0, -1)),
			east = map.peek(getPosition() + Tuple(1, 0)),
			west = map.peek(getPosition() + Tuple(1, -1));

		unsigned int sensorState = 0x0;
		sensorState = sensorState | (north << 0x6);
		sensorState = sensorState | (south << 0x4);
		sensorState = sensorState | (east << 0x2);
		sensorState = sensorState | west;

#ifdef DEBUG
		cout << north << " " << south << " " << east << " " << west << endl;
		cout << "Bits: " << bitset<8>(sensorState) << endl;
#endif

		return getInstruction(sensorState);
	};
	void compute(const Map& map) {
		GeneInstruction instruction = static_cast<GeneInstruction>(useSensor(map));
		Tuple newPosition(getPosition());
		if (instruction == GeneInstruction::MOVE_RANDOM) {
			instruction = static_cast<GeneInstruction>(rand() % 0x4);
		}
		switch(instruction) {
			case MOVE_NORTH:
				newPosition += Tuple(0, 1);
				break;
			case MOVE_SOUTH:
				newPosition += Tuple(0, -1);
				break;
			case MOVE_EAST:
				newPosition += Tuple(1, 0);
				break;
			case MOVE_WEST:
				newPosition += Tuple(-1, 0);
				break;
			default:
				break;
		}
		Square val = map.peek(newPosition);
		if (val != Square::WALL) {
			setPosition(newPosition);
		}
		if (val == Square::BATTERY) {
			setPower(getPower() + BATTERY_POWER_GAINED);
			powerHarvested++;
		}

		setPower(getPower() - BATTERY_POWER_LOST_PER_MOVE);
		turnsSurvived++;
	};

};

void UnitTest();

// Print a c-string
ostream& printStr(ostream& out, const char* str) {
	for (int i = 0; i < strlen(str); i++) {
		out << str[i];
	}
	return out;
}

// Returns the index of a startup parameter, if it exists, or -1 if it doesn't
int getStartupParameter(int argc, char** argv, const char* value) {
	for (int i = 0; i < argc; i++) {
		printStr(cout, argv[i]) << endl;
		if (!strcmp(argv[i], value))
			return i;
	}
	return -1;
}

Map* readFileToMap(istream& in);

int main(int argc, char** argv) {
	if (getStartupParameter(argc, argv, "test") != -1) {
		cout << "Testing!" << endl;
		UnitTest();
		return 0;
	}

	srand(time(nullptr));

	Map* map;

	int mapFileInput = getStartupParameter(argc, argv, "mapfile");
	if (mapFileInput != -1) {
		if (argc <= mapFileInput) {
			cout << "Map file not defined!" << endl;
			return 1;
		}
		const char* fileName = argv[mapFileInput + 1];
		cout << "Map file loaded: " << fileName << endl;
		ifstream mapFile(fileName);

		map = readFileToMap(mapFile);
	} else {
		map = new Map(Tuple(10, 10));
	}

	cout << "Map file: " << endl << *map << endl;

	ArrayList<Robot*>* robots = new ArrayList<Robot*>();
	for (int i = 0; i < 10; i++) {
		robots->push(new Robot(map->getStartSquare()));
	}
	Robot* robot = new Robot(map->getStartSquare());

	while(robot->getPower() > 0) {
		robot->compute(*map);
	}
	cout << "This robot survived " << robot->getTurnsSurvived() << " turns!";

	return 0;
}

void UnitTest() {
	ArrayList<void*>::ComponentTest();
	Date::ComponentTest();
	Assert::analyze();
}

Map* readFileToMap(istream& in) {
	// Read dimensions first
	int x, y;
	in >> x >> y;
	cout << "(" << x << ", " << y << ")" << endl;
	Map* map = new Map(Tuple(x, y));

	int k = 0;
	string line;
	// clear the newline after the header
	getline(in, line);
	while (getline(in, line)) {
		for (int i = 0; i < x; i++) {
			Square square;
			char c = line.at(i);
			switch (c) {
				case '#':
					square = Square::WALL;
					break;
				case 'B':
					square = Square::BATTERY;
					break;
				case 'S':
					map->setStartSquare(Tuple(y - k - 1, i));
				default:
					square = Square::NOTHING;
					break;
			}
			map->setSquare(Tuple(y - k - 1, i), square);
		}
		k++;
	}

	return map;
}