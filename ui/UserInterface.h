#pragma once
#include <string>
#include "../models/File.h"
#include "../services/FileManager.h"
#include "../services/ConnectionManager.h"
#include "../lib/NullPointerException.h"
#include <thread>
#include <memory>
#include <iostream>
using namespace std;

class UserInterface
{
	FileManager *fileManager;
	ConnectionManager *connectionManager;
	bool isMultiMode;
	void sendFile(string fileName);
	void receiveFile();
public:
	UserInterface();
	File *loadFile(string file);
	void send();
	string chooseFile();
	void init();
	int showMenu();
	bool initPort();
	bool setMode();
	~UserInterface();
};