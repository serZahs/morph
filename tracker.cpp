#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "tracker.h"
using std::string;
namespace fs = std::experimental::filesystem;

void Sorter::initDirectory() {
	std::ofstream outfile (".watch");
	outfile << fs::absolute(workingDir) << "\n";
	outfile.close();
}
