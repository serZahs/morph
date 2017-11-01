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
	string fileExtension;
	string destinationPath;
	std::ifstream infile;
	infile.open (".watch");
	while (infile >> word) {
		/* .watch stores filters in the following format:
		 * 
		 * <file extension> <destination for all files with extension>
		 * e.g.
		 * .jpg ~/Pictures
		 *
		 * so file extension must be accessed first and then
		 * the path of the destination directory
		 */
		string currentFile;
		string currentFileExtension;
		fileExtension = word;
		infile >> word;
		destinationPath = word;
		filters[fileExtension] = destinationPath;

		for (auto &p : fs::directory_iterator(workingDir)) {
			currentFileExtension = fs::path(p).extension(); 
			currentFile = fs::path(p).filename();
			if (!(filters.find(currentFileExtension) == filters.end())) {
				fs::rename(fs::path(p), filters[currentFileExtension]+"/"+currentFile);
			}
		}
	}
	infile.close();
}
