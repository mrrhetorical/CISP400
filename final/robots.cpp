/**
 * Todo: Remove debug defines and includes
 */

#define BATTERY_POWER_GAINED 5
#define BATTERY_POWER_LOST_PER_MOVE 1
#define BATTERY_SPAWN_RATE 0.8f

#ifdef DEBUG
#include <bitset>
#endif

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <functional>
#include <regex>

typedef unsigned int Gene;

using namespace std;

#include "../cbrock.cpp"

using namespace cbrock;

// Simple tuple class. Easier than passing in x and y each time to every function. Allows for some basic addition and comparison.
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
	bool operator==(const Tuple& other) const {
		return this->getX() == other.getX() && this->getY() == other.getY();
	};
	Tuple& operator=(const Tuple& other) {
		setX(other.getX());
		setY(other.getY());
		return *this;
	};
	Tuple operator+(const Tuple& other) const {
		return {getX() + other.getX(), getY() + other.getY()};
	};
	Tuple operator+=(const Tuple& other) {
		*this = *this + other;
		return *this;
	};
	friend ostream& operator<<(ostream& out, const Tuple& tuple) {
		out << "(" << tuple.getX() << ", " << tuple.getY() << ")";
		return out;
	};
};

// This is used to store only particular values within the map
enum Square {
	NOTHING = 0x0,
	WALL = 0x1,
	BATTERY = 0x2
};

// This is used to get the specific parts of the gene if needed
enum GeneMask {
	GENES = 0xff,
	GENE_NORTH = 0xc0,
	GENE_SOUTH = 0x30,
	GENE_EAST = 0xc,
	GENE_WEST = 0x3,
	INSTRUCTION = 0xf000
};

// This is an enum to decode the different gene instructions
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
			return Square::WALL;
		}

		return this->squares[coords.getY()][coords.getX()];
	};
	void setSquare(const Tuple& pos, Square value) {
		if (!isValidPosition(pos))
			throw out_of_range("The coordinates are out of range!");

		this->squares[pos.getY()][pos.getX()] = value;
	};
	bool isValidPosition(const Tuple& coords) const {
		return !(coords.getX() < 0 || coords.getX() >= this->dimensions.getX() || coords.getY() < 0 || coords.getY() >= this->dimensions.getY());
	};
	const Tuple& getStartSquare() const {
		return this->startSquare;
	};
	void setStartSquare(const Tuple& value) {
		this->startSquare = value;
	};
	void placeBatteries(float spawnRate, bool reset = false) {
		if (reset) {
			for (int y = 0; y < this->dimensions.getY(); y++) {
				for (int x = 0; x < this->dimensions.getX(); x++) {
					if (this->peek(Tuple(x, y)) == Square::BATTERY) {
						this->setSquare(Tuple(x, y), Square::NOTHING);
					}
				}
			}
		}

		for (int y = 0; y < this->dimensions.getY(); y++) {
			for (int x = 0; x < this->dimensions.getX(); x++) {
				if (this->peek(Tuple(x, y)) != Square::NOTHING)
					continue;

				float r = static_cast<float>(rand() % 100) / 100.0f;
				if (r > spawnRate) {
					this->setSquare(Tuple(x, y), Square::BATTERY);
				}
			}
		}
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

template<typename T> void bubbleSort(ArrayList<T>*, function<int(T, T)>);

// Self-explanatory class
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
	Gene* genes;
	int geneCount;
	int power;
	Tuple position;
	int powerHarvested;
	int turnsSurvived;
	int id;


	// Returns how many bits of the instructions DON'T match! Lower is closer match!
	static int compareGene(Gene a, Gene b) {
		// This compares just the last 4 bits of the byte (11111111 = 0xff)
		int quality = 0;
		if (!geneMatches(a, b, GeneMask::GENE_NORTH)) {
			quality++;
		}
		if (!geneMatches(a, b, GeneMask::GENE_SOUTH)) {
			quality++;
		}
		if (!geneMatches(a, b, GeneMask::GENE_EAST)) {
			quality++;
		}
		if (!geneMatches(a, b, GeneMask::GENE_WEST)) {
			quality++;
		}
		return quality;
	};
	// Returns true if the part of the gene specified matches!
	static bool geneMatches(const Gene a, const Gene b, const GeneMask geneMask) {
		return (a & geneMask) == (b & geneMask);
	};
	static Gene getInstructionFromGene(Gene gene) {
		// Bit-shifting a 32-bit unsigned integer by 0x1b units leaves only the last 4 bits
		return gene >> 0xc;
	};
	static Gene generateGene() {
		Gene gene = 0;
		for (int k = 0; k < 4; k++) {
			Gene sensorGeneValue = rand() % 0x3; // random number between 0-3 (00, 01, 10, 11)
			/* 0000 0000 0000 0000
			 * 0100
			 * 1100
			 * 000000
			 * 01000000
			 * 0100000000
			 * */
			sensorGeneValue <<= 0x2 * k; // Shift to 0-3
			gene = gene | sensorGeneValue; // Bitwise OR to set new gene state
		}  // This generates the genes
		Gene outputGeneValue = rand() % 0x4; // random number between 0-4
		outputGeneValue <<= 0xc; // Shift it 12 bits left
		gene = gene | outputGeneValue;
		return gene;
	}
	void generateGenes(int count) {
		genes = new Gene[count];
		for (int i = 0; i < count; i++) {
			genes[i] = generateGene();
		}
	}
public:
	Robot(const Tuple& startingPosition = Tuple(0, 0), int startingPower = 10, int geneCount = 16) {
		this->id = rand() & 0xFFFF;
		setPower(startingPower);
		setPosition(startingPosition);
		this->geneCount = geneCount;
		generateGenes(geneCount);
		setPowerHarvested(0);
		setTurnsSurvived(0);
	};
	~Robot() {
		delete [] genes;
	};
	void copyGenes(const Robot* other, int start, int length) {
		copy(other->genes + start, other->genes + start + length, this->genes + start);
	};
	void mutateGene(int idx) {
		// Randomly either frameshift or generate a new gene on mutation
		if (rand() % 2 == 0) {
			// Frameshift with wraparound
			int totalBits = sizeof(Gene) * 8;
			int shiftAmt = rand() % totalBits;
			if (rand() % 2 == 0) {
				genes[idx] = (genes[idx] << shiftAmt) | (genes[idx] >> (totalBits - shiftAmt));
			} else {
				genes[idx] = (genes[idx] >> shiftAmt) | (genes[idx] << (totalBits - shiftAmt));
			}
		} else {
			genes[idx] = generateGene();
		}
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
	void setPowerHarvested(int value) {
		this->powerHarvested = value;
	}
	int getTurnsSurvived() const {
		return this->turnsSurvived;
	};
	int getId() const {
		return this->id;
	};
	void setTurnsSurvived(int value) {
		this->turnsSurvived = value;
	}
	const Tuple& getPosition() const {
		return this->position;
	};
	void setPosition(const Tuple& value) {
		this->position = value;
	};
	// Gets the instruction part from a gene
	Gene getInstruction(Gene sensors) {
		int match;
		struct GeneQuality {
			int index;
			int quality;
		};
		ArrayList<GeneQuality>* candidates = new ArrayList<GeneQuality>;
		for (int i = 0; i < geneCount; i++) {
			candidates->push(GeneQuality{i, compareGene(genes[i], sensors)});
		}

		// Sort in ascending order (better quality / lowest number first)
		bubbleSort<GeneQuality>(candidates, [](GeneQuality a, GeneQuality b) {
			if (a.quality > b.quality)
				return 1;
			if (a.quality < b.quality)
				return -1;
			return 0;
		});

		Gene bestMatchGene = getInstructionFromGene(genes[candidates->at(0).index]);

		delete candidates;

		return bestMatchGene;
	};
	Gene useSensor(const Map& map) {
		// Check north
		Square north = map.peek(getPosition() + Tuple(0, 1)),
			south = map.peek(getPosition() + Tuple(0, -1)),
			east = map.peek(getPosition() + Tuple(1, 0)),
			west = map.peek(getPosition() + Tuple(-1, 0));

		Gene sensorState = 0x0;
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
	void compute(Map& map) {
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
			map.setSquare(newPosition, Square::NOTHING);
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

/**
 * Returns the index of a startup parameter, if it exists, or -1 if it doesn't.
 * There's probably a better way to do this, but I just wanted to allow the startup parameters to ignore order.
*/
int getStartupParameter(int argc, char** argv, const char* value) {
	for (int i = 0; i < argc; i++) {
		printStr(cout, argv[i]) << endl;
		if (!strcmp(argv[i], value))
			return i;
	}
	return -1;
}

Map* readFileToMap(istream& in);
void produceNewGeneration(ArrayList<Robot*>*, int = 16);
void resetRobotData(ArrayList<Robot*>*, const Tuple&, int);

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
		map->setStartSquare(Tuple(5, 5));
	}

	map->placeBatteries(BATTERY_SPAWN_RATE, true);

	cout << "Map: " << endl << *map << endl;

	cout << "How many generations do you want to run?" << endl;
	int generations;
	do {
		if (!(cin >> generations)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter a valid generation count:" << endl;
			continue;
		}

		break;
	} while (true);

	int generationSize;
	cout << "How big should your generation be?" << endl;
	do {
		if (!(cin >> generationSize) || generationSize % 4 != 0) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter a valid generation count:" << endl;
			continue;
		}
		break;
	} while (true);

	// Initialize generation 0
	ArrayList<Robot*>* robots = new ArrayList<Robot*>();
	for (int i = 0; i < generationSize; i++) {
		Robot* r = new Robot(map->getStartSquare());
		robots->push(r);
	}

	cout << "Generation 0 initialized" << endl;

	ArrayList<Tuple>* outputData = new ArrayList<Tuple>();

	for (int generation = 0; generation < generations; generation++) {
		int harvested = 0;
		for (int i = 0; i < robots->length(); i++) {
			map->placeBatteries(BATTERY_SPAWN_RATE, true);
			Robot *robot = robots->at(i);
			while (robot->getPower() > 0) {
				robot->compute(*map);
			}
			harvested += robot->getPowerHarvested();
		}
		outputData->push(Tuple(generation, harvested));
#ifdef DEBUG
		cout << "Robots of generation " << generation << " harvested " << harvested << " power!" << endl;
#endif
		if ((generation > 0 && generation % 100 == 0) || generation == generations - 1) {
			cout << "Generation up to " << generation << " finished..." << endl;
		}
		produceNewGeneration(robots);
		resetRobotData(robots, map->getStartSquare(), 10);
	}

	cout << "Finished running all " << generations << " generations!" << endl
		<< "Would you like to see the results? (y/n)" << endl;


	// Clean up previous inputs
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	string results;
	getline(cin, results);

	if (regex_match(results, regex("(yes)|(y)", regex_constants::icase))) {
		cout << "Writing contents to results.csv..." << endl;
		ofstream outputFile("results.csv");
		if (!outputFile) {
			cerr << "Error opening file results.csv" << endl;
			return 1;
		}
		for (int i = 0; i < outputData->length(); i++) {
			Tuple tuple = outputData->at(i);
			outputFile << tuple.getX() << "," << tuple.getY() << endl;
		}
		outputFile.close();
		cout << "Contents written to disk!" << endl;
	}

	delete outputData;

	return 0;
}

/**
 * Orders in ascending order as written, but you could just flip the compare() function's logic to achieve the opposite effect.
 * I love using lambdas like this in C# and Java so I thought it'd be fun to try it in C++
 *
 * How to use compare() function:
 * If compare()'s output is < 0 then first number is smaller
 * If compare()'s output is > 0 then the first number is greater
 * If compare()'s output is = 0 then the numbers are equal
 * */
template <typename T> void bubbleSort(ArrayList<T>* arr, function<int(T, T)> compare) {
	for (int i = 0; i < arr->length() - 1; i++) {
		if (compare(arr->at(i), arr->at(i + 1)) > 0) {
			T tmp = arr->at(i + 1);
			arr->put(i + 1, arr->at(i));
			arr->put(i, tmp);
		}
	}
}

void resetRobotData(ArrayList<Robot*>* robots, const Tuple& startingPosition, int startingPower) {
	for (int i = 0; i < robots->length(); i++) {
		Robot* r = robots->at(i);
		r->setPower(startingPower);
		r->setPosition(startingPosition);
		r->setTurnsSurvived(0);
		r->setPowerHarvested(0);
	}
}

void produceNewGeneration(ArrayList<Robot*>* robots, int geneCount) {
	// Sort by power harvested in ascending order
	bubbleSort<Robot*>(robots, [](Robot* x, Robot* y) {
		if (x->getPowerHarvested() > y->getPowerHarvested())
			return 1;
		else if (x->getPowerHarvested() < y->getPowerHarvested())
			return -1;
		return 0;
	});

	int halfSize = robots->length() / 2;
	for (int i = 0; i < halfSize; i++) {
		if (i < halfSize) {
			delete robots->at(0);
			robots->remove(0);
		}
	}

	int pairs = robots->length() / 2;
	for (int i = 0; i < pairs; i++) {
		Robot* parentA = robots->at(i * 2),
			*parentB = robots->at(i * 2 + 1);

		Robot* x = new Robot(),
			*y = new Robot();

		auto tryToMutate = [geneCount](Robot* r) {
			// Mutate 5% of the time
			if (rand() % 100 < 5) {
				r->mutateGene(rand() % geneCount);
			}
		};

		x->copyGenes(parentA, 0, geneCount / 2);
		x->copyGenes(parentB, geneCount / 2, geneCount / 2);

		y->copyGenes(parentA, geneCount / 2, geneCount / 2);
		y->copyGenes(parentB, 0, geneCount / 2);

		tryToMutate(x);
		tryToMutate(y);

		robots->push(x);
		robots->push(y);
	}
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