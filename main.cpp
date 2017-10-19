#include <iostream>
#include "tracker.h"

const char watch_error[] = "Type 'watch' for avaiable commands.\n";

int main (int argc, char *argv[]) {
	if (argc > 3) {
		std::cerr << "Error: too many arguments. ";
		std::cerr << watch_error;
		return -1;
	}
	Sorter demo;
	demo.initDirectory();

}
