#include <iostream>
#include <cstring>
#include <string>
#include "tracker.h"
using std::string;

const string watch_err = "Type 'watch' for available commands.\n";

int main (int argc, char *argv[]) {
	if (argc > 4) {
		std::cerr << "Error: too many arguments. ";
		std::cerr << watch_err;
		return -1;
	}
	Sorter demo;
	demo.initDirectory();
	if (strcmp(argv[1],"add") == 0) {
		demo.addFilter(string(argv[2]),string(argv[3]));
		std::cout << "Added " << argv[2] << " to .watch file.\n";
	}
	demo.cleanUp();

}
