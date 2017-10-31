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
	outfile.open (".watch");
	outfile << fileExt << " " << destinationPath << "\n";
	outfile.close();	
}

void Sorter::cleanUp() {
	/* for each line in .watch
	 * all the files with respective file extension
	 * move to appropriate folder
	 * loop
	 */
	string word;
	std::ifstream infile;
	infile.open (".watch");
	while (infile >> word) {
		std::cout << word << " ";	
	}
	infile.close();
}
