#include <iostream>
#include <cstring>
#include <string>
#include "tracker.h"
using std::string;

const string watch_err = "Type 'watch' for available commands.\n";
const string watch_cmds = "List of available watch commands\n"
			  "  init\t\tInitiliaze a directory\n"
			  "  add\t\tAdd a filter\n"
			  "  sort\t\tApply rules to sort files\n";

int main (int argc, char *argv[]) {

	Sorter demo;

	if (argc > 1) {
		const string command(argv[1]); 
		if (command == "init") {
			demo.initDirectory();
			return 0;
		} else if (command == "add") {
			demo.addFilter(static_cast<string>(argv[2]),
				       static_cast<string>(argv[3])); 
			std::cout << argv[2] << 
				  " files will now be routed towards " <<
				  argv[3] << "\n";
			return 0;
		} else if (command == "sort") {
			demo.cleanUp();
			return 0;
		} 
	} else {
		std::cout << watch_cmds;
	}
}
