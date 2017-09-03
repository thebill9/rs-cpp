#include "UserInterface.h"

void UserInterface::sendFile(string fileName)
{
	File *file = loadFile(fileName);
	this->connectionManager->sendFile(file);
}

void UserInterface::receiveFile()
{
	cout << "start reading\n";
	File *file = this->connectionManager->receiveFile();
	this->fileManager->save(file);
	if (file) {
		cout << "received in UI " << file->getFileName() << endl;
	}
	cout << "end reading\n";
}

UserInterface::UserInterface()
{

}

File *UserInterface::loadFile(string file)
{
	if (!this->fileManager) {
		throw new NullPointerException;
	}

	File *data = this->fileManager->load(file);
	return data;
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

bool UserInterface::initPort()
{
	char portname[5] = "COM";
	char c;
	cout << "Numer portu COM do otwarcia: ";
	cin >> c;
	portname[3] = c;
	cout << "otwieranie: " << portname << endl;
	string tryToOen = "y";
	while ((tryToOen == "y" || tryToOen == "Y" || tryToOen == "yes" || tryToOen == "YES" || tryToOen == "Yes")
		&& !this->connectionManager->init(portname)) {
		cout << "Nie udalo sie otworzyc portu " << portname << endl << "Sprobowac ponownie (y/n)?\n";
		cin >> tryToOen;
	}
	return connectionManager->isPortOpen();
}

void UserInterface::init()
{

	if (!setMode()) {
		return;
	}
	this->fileManager = new FileManager();
	this->connectionManager = new ConnectionManager();

	if (!this->initPort()) {
		return;
	}


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
	//    system("cls");
	cout << "Wybierz akcje:\n";
	cout << "(0) zakoncz prace\n";
	cout << "(1) wysylanie pliku\n";
	if (!this->isMultiMode) {
		cout << "(2) odbieranie pliku\n";
	}
	int decision;
	cin >> decision;
	switch (decision) {
	case 1:
		this->send();
		break;
	case 2:
		if (!this->isMultiMode) {
			this->receiveFile();
		}

		break;
	}
	return decision;
}

bool UserInterface::setMode()
{
	while (true) {
		cout << "Tryb programu\n"
			"(1) jednowatkowy\n"
			"(2) wielowatkowy\n"
			"(0) zakoncz prace\n";
		int decision;
		cin >> decision;
		switch (decision) {
		case 0:
			return false;
		case 1:
			this->isMultiMode = false;
			return true;
		case 2:
			this->isMultiMode = true;
			return true;
		}
	}
}

UserInterface::~UserInterface()
{

	if (this->connectionManager) {
		delete connectionManager;
	} if (this->fileManager) {
		delete fileManager;
	}
}
