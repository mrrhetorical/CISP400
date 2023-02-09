// hokeemon.cpp
// Caleb Brock, CISP 400
// April 23, 2023

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "../cbrock.cpp"

using namespace std;

using namespace cbrock;

void UnitTest();

int main(int argc, char** argv) {
	if (argc > 1) {
		if (!strcmp(argv[1], "test")) {
			cout << "Testing!" << endl;
			UnitTest();
			return 0;
		}
	}

	cout << FancyText(AsciiColor::BLUE) << "This text should be blue!" << FancyText(AsciiColor::CLEAR) << endl << "And this should be back to normal!" << endl;

	return 0;
}

void UnitTest() {
	Date::ComponentTest();
	ArrayList<void*>::ComponentTest();
	Assert::analyze();
}