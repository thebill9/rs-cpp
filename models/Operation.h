#include <string>
#include <chrono>
using namespace std;

class Operation {
public:
	string name;
	chrono::time_point<chrono::system_clock> start;
	chrono::time_point<chrono::system_clock> end;
	Operation(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end, string name);
};