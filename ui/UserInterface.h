#pragma once
#include <string>
#include "../models/File.h"
#include "../services/FileManager.h"
#include "../services/ConnectionManager.h"
#include "../lib/NullPointerException.h"
#include "../models/Operation.h"
#include <thread>
#include <memory>
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;

class UserInterface
{
	mutex operationsMutex;
	vector<Operation> operations;
	string modeStr;
	string portStr;
	FileManager *fileManager;
	ConnectionManager *connectionManager;
	bool isMultiMode;
	void sendFile(string fileName);
	void receiveFile();
	void saveOperation(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end, string name);
	void printOperations();
	void updateTitle();
	void displayMainMenu();
public:
	UserInterface();
	File *loadFile(string file);
	void send(char * filename);
	void send();
	string chooseFile();
	void init(char * mode, char * portNr, char * operation, char * path);
	void init();
	int showMenu();
	bool setMode(int mode);
	bool initPort(char portNumber);
	bool initPort();
	bool setMode();
	~UserInterface();
};