#include "../models/Port.h"
#include "../models/File.h"
#include <string>
#include "../lib/NullPointerException.h"
#include <thread>
#include <memory>
#include <iostream>
#include <chrono>

using namespace std;

class ConnectionManager
{
	Port *port;
	size_t sendData(char * data, size_t size, chrono::time_point<chrono::system_clock>*& start, chrono::time_point<chrono::system_clock>*& end);
	void parse2send(File *file, char *&output, size_t &size);
	string filenameMessage = "filename:";
	File *parseData(char *data, size_t size);
public:
	ConnectionManager();
	bool isPortOpen();
	bool init(string portName);
	size_t sendFile(File * file, chrono::time_point<chrono::system_clock>*& start, chrono::time_point<chrono::system_clock>*& end);
	File * receiveFile(chrono::time_point<chrono::system_clock>*& start, chrono::time_point<chrono::system_clock>*& end);
	bool canRead();

	~ConnectionManager();
};