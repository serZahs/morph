#ifndef tracker_h
#define tracker_h
#include <string>
#include <vector>
#include <map>
#include <experimental/filesystem>
using std::string; using std::vector; using std::map;
namespace fs = std::experimental::filesystem;

class Sorter {
	//const string fileExtension;
	fs::path workingDir = fs::current_path();
	map<string, string> filters;
public:
	void initDirectory();
	void addFilter(string fileExt, string destinationPath);
	void cleanUp();
};
#endif /* tracker_h */
