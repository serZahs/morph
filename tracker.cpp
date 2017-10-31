#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "tracker.h"
using std::string;
namespace fs = std::experimental::filesystem;

void Sorter::initDirectory() {
	std::ofstream outfile (".watch");
	/*outfile << fs::absolute(workingDir) << "\n";*/
	outfile.close();
}

void Sorter::addFilter(string fileExt, string destinationPath) {
	std::ofstream outfile;
	outfile.open (".watch", std::ios::app);
	outfile << fileExt << " " << destinationPath << "\n";
	outfile.close();	
}

void Sorter::cleanUp() {
	string word;
	std::ifstream infile;
	infile.open (".watch");
	while (infile >> word) {
		// for each file in folder	
			// move file to appropriate folder	
		// loop
	}
	infile.close();
}
