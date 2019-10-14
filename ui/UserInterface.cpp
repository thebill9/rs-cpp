#include "UserInterface.h"
#include <Windows.h>
#include <chrono>
#include <limits>
#include <cmath>

void UserInterface::sendFile(string fileName)
{
	try {

		chrono::time_point<chrono::system_clock> loadStart, loadEnd;
		chrono::time_point<chrono::system_clock> *sendStart, *sendEnd;

	loadStart = chrono::system_clock::now();
	File *file = loadFile(fileName);
	loadEnd = chrono::system_clock::now();
	if (file) {
	string operationName = "wczytywanie pliku: " + file->getFileName();
	this->saveOperation(loadStart, loadEnd, operationName);
		system("cls");
		string fileName = file->getFileName();
		size_t filesize = file->getSize();
		cout << "zaladowano plik " << fileName << " (";
		if (filesize < 1024) {
			cout << filesize << " B)" << endl;
		}
		else if (filesize / 1024 < 1024) {
			float result = (float)filesize / 1024.0;
			cout << roundf(result * 100) * 0.01 << " KB)" << endl;
		}
		else {
			float result = (float)filesize / 1048576.0;
			cout << roundf(result * 100) * 0.01 << " MB)" << endl;
		}
		displayMainMenu();
		this->connectionManager->sendFile(file, sendStart, sendEnd);
		operationName = "wysylanie pliku: " + fileName;
		this->saveOperation(*sendStart, *sendEnd, operationName);

		system("cls");
		cout << "zaladowano plik " << fileName << " (";
		if (filesize < 1024) {
			cout << filesize << " B)" << endl;
		}
		else if (filesize / 1024 < 1024) {
			float result = (float)filesize / 1024.0;
			cout << roundf(result * 100) * 0.01 << " KB)" << endl;
		}
		else {
			float result = (float)filesize / 1048576.0;
			cout << roundf(result * 100) * 0.01 << " MB)" << endl;
		}
		cout << "wyslano\n";
		displayMainMenu();
	}

	}
	catch (exception *e) {
		cout << e->what() << endl;
		delete e;
	}

}

void UserInterface::receiveFile()
{
	chrono::time_point<chrono::system_clock> *receiveStart, *receiveEnd;
	chrono::time_point<chrono::system_clock> saveStart, saveEnd;
	File *file = this->connectionManager->receiveFile(receiveStart, receiveEnd);

	if (file) {
		string operationName = "odbieranie pliku: " + file->getFileName();
		this->saveOperation(*receiveStart, *receiveEnd, operationName);
		system("cls");
		cout << "odebrano plik: " << file->getFileName() << " (";
		if (file->getSize() < 1024) {
			cout << file->getSize() << " B)" << endl;
		}
		else if (file->getSize() / 1024 < 1024) {
			float result = (float)file->getSize() / 1024.0;
			cout << roundf(result * 100) * 0.01 << " KB)" << endl;
		}
		else {
			float result = (float)file->getSize() / 1048576.0;
			cout << roundf(result * 100) * 0.01 << " MB)" << endl;
		}
		displayMainMenu();
		saveStart = chrono::system_clock::now();
			this->fileManager->save(file);

		saveEnd = chrono::system_clock::now();
		operationName = "zapisywanie pliku: " + file->getFileName();
		this->saveOperation(saveStart, saveEnd, operationName);
		delete file;
	}
	if (this->isMultiMode) {
		receiveFile();
	}
}

void UserInterface::saveOperation(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end, string name) {
	Operation op(start, end, name);
	lock_guard<mutex> guard(this->operationsMutex);
	this->operations.push_back(op);
}

void UserInterface::printOperations() {
	system("cls");
	if (this->operations.empty()) {
		cout << "Brak operacji do wyswietlenia\n";
	}
	else {
		chrono::time_point<chrono::system_clock> zero = chrono::system_clock::now();
		chrono::time_point<chrono::system_clock> totalStart = zero;
		chrono::time_point<chrono::system_clock> totalEnd = zero;
		__int64 total = 0;


		for (int i = 0; i < this->operations.size(); ++i) {
			cout <<"(" << i+1 << ") " << this->operations[i].name << endl;
			//cout << "\trozpoczeto: " << ctime(&this->operations[i].start) << endl;
			//cout << "\tzakonczono: " << ctime(&this->operations[i].end) << endl;
			auto operationElapsed = chrono::duration_cast<chrono::milliseconds>(
				this->operations[i].end 
				- this->operations[i].start
				);
			total += operationElapsed.count();
			cout << "\tczas trwania: " << operationElapsed.count() << "ms\n";

			if (totalStart == zero || this->operations[i].start < totalStart) {
				totalStart = this->operations[i].start;
			}
			if (totalEnd == zero || this->operations[i].end > totalEnd) {
				totalEnd = this->operations[i].end;
			}
		}
		auto totalElapsed = chrono::duration_cast<chrono::milliseconds>(
			totalEnd
			- totalStart
			);
		//cout << "inne operacje: " << totalElapsed.count() - total << "ms\n";
		cout << "czas wykonywania wszystkich operacji: " << totalElapsed.count() << "ms\n";
	}
	displayMainMenu();
}

void UserInterface::updateTitle()
{
	string base = "RS";
	if (!modeStr.empty() || !portStr.empty()) {
		base += " (";
		if (!modeStr.empty()) {
			base += modeStr;
		} if (!portStr.empty()) {
			base += ", " + portStr;
		}
		base += ")";
	}
	SetConsoleTitle(base.c_str());
}

void UserInterface::displayMainMenu()
{
	cout << "\nWybierz akcje:\n";
	cout << "(0) zakoncz prace\n";
	cout << "(1) statystyki\n";
	cout << "(2) wysylanie pliku\n";
	if (!this->isMultiMode) {
		cout << "(3) odbieranie pliku\n";
	}
}

UserInterface::UserInterface()
{
	this->connectionManager = nullptr;
	this->fileManager = nullptr;

}

File *UserInterface::loadFile(string file)
{
	if (!this->fileManager) {
		throw new NullPointerException;
	}

	File *data = this->fileManager->load(file);
	return data;
}

void UserInterface::send(char* filename)
{
	try {
		string fileName(filename);
		//if (this->isMultiMode) {
		//	cout << "detaching write\n";
		//	thread t(&UserInterface::sendFile, this, fileName);
		//	t.detach();
		//}
		//else {
		//
			this->sendFile(fileName);
		//}

	}
	catch (exception *e) {
		cout << e->what() << endl;
		delete e;
	}
}

void UserInterface::send()
{
	try {
		string fileName = chooseFile();
		if (this->isMultiMode) {
			thread t(&UserInterface::sendFile, this, fileName);
			t.detach();
		}
		else {
			this->sendFile(fileName);
		}

	}
	catch (exception *e) {
		cout << e->what() << endl;
		delete e;
	}
}

string UserInterface::chooseFile()
{
	string fileName;
	cout << "Sciezka do pliku: ";
	cin >> fileName;
	return fileName;
}


bool UserInterface::initPort(char portNumber) {
	char portname[5] = "COM";
	portname[3] = portNumber;

	return this->connectionManager->init(portname);
}

bool UserInterface::initPort()
{
	char portname[5] = "COM";
	char c;
	cout << "Numer portu COM do otwarcia: ";
	cin >> c;
	portname[3] = c;
	string tryToOen = "t";
	while ((tryToOen == "t" || tryToOen == "T" || tryToOen == "tak" || tryToOen == "TAK" || tryToOen == "Tak")
		&& !this->connectionManager->init(portname)) {
		cout << "Nie udalo sie otworzyc portu " << portname << endl << "Sprobowac ponownie (t/n)?\n";
		cin >> tryToOen;
	}
	if (connectionManager->isPortOpen()) {
		this->portStr = portname;
	}
	return connectionManager->isPortOpen();
}

void UserInterface::init(char *mode, char *portNr, char *operation, char *path) {
	if (!setMode(atoi(mode))) {
		return;
	}

	this->fileManager = new FileManager();
	this->connectionManager = new ConnectionManager();

	if (!this->initPort(portNr[0])) {
		return;
	}
	
	this->updateTitle();
	if (this->isMultiMode) {
		thread t(&UserInterface::receiveFile, this);
		t.detach();
	}
		switch (operation[0]) {
		case '0':
			return;
		case '1':
			printOperations();
			break;
		case '2':
			this->send(path);
			break;
		case '3':
			if (!this->isMultiMode) {
				this->receiveFile();
			}
			break;
		}

		this->printOperations();
}
void UserInterface::init()
{
	this->updateTitle();
	if (!setMode()) {
		return;
	}
	this->updateTitle();
	this->fileManager = new FileManager();
	this->connectionManager = new ConnectionManager();

	if (!this->initPort()) {
		return;
	}
	system("cls");
	displayMainMenu();
	this->updateTitle();


	if (this->isMultiMode) {

		thread t(&UserInterface::receiveFile, this);
		t.detach();
	}
	int decision = -1;
	while (decision != 0) {
		decision = showMenu();
	}
}

int UserInterface::showMenu()
{
	int decision;
	cin >> decision;
	switch (decision) {
	case 1:
		this->printOperations();
		break;
	case 2:
		this->send();
		break;
	case 3:
		if (!this->isMultiMode) {
			this->receiveFile();
		}
		break;
	}
	return decision;
}

bool UserInterface::setMode(int mode) {
	switch (mode) {
	case 0:
		return false;
	case 1:
		this->modeStr = "jednowatkowy";
		this->isMultiMode = false;
		return true;
	case 2:
		this->modeStr = "wielowatkowy";
		this->isMultiMode = true;
		return true;
	}
}
bool UserInterface::setMode()
{
		cout << "Tryb programu\n"
			"(0) zakoncz prace\n"
			"(1) jednowatkowy\n"
			"(2) wielowatkowy\n";
		int decision;
		cin >> decision;
		return this->setMode(decision);
}

UserInterface::~UserInterface()
{

	if (this->connectionManager) {
		delete connectionManager;
	} if (this->fileManager) {
		delete fileManager;
	}
}
