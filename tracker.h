#ifndef tracker_h
#define tracker_h
#include <string>
#include <vector>
#include <experimental/filesystem>
using std::string; using std::vector;
namespace fs = std::experimental::filesystem;

class Sorter {
	//const string fileExtension;
	fs::path workingDir = fs::current_path();
public:
	void initDirectory();
};
#endif /* tracker_h */
