#include "FileManager.h"
#include <iostream>

bool FileManager::fileExist(string filePath)
{
	struct stat buffer;
	return (stat(filePath.c_str(), &buffer) == 0);
}

string FileManager::getValidFileName(string fileName)
{
	unsigned i = 1;
	string newName = fileName;
	size_t indexOfDot = fileName.find_last_of('.');
	indexOfDot = indexOfDot == string::npos ? fileName.size() : indexOfDot;
	while (fileExist(newName)) {
		newName = fileName.substr(0, indexOfDot)
			+ "(" + to_string(i) + ")"
			+ fileName.substr(indexOfDot);
		++i;
		cout << "inProgress: " + fileName.substr(0, indexOfDot)
			+ ", to: " + fileName.substr(indexOfDot) << endl;
	}
	cout << newName;
	return newName;
}

string FileManager::extractFileName(string filePath)
{
	size_t position = filePath.find_last_of('/');
	if (position == string::npos) {
		return filePath;
	}
	return filePath.substr(position + 1);
}

File *FileManager::extractData(ifstream &inFile, string fileName)
{
	if (!inFile.is_open()) {
		return nullptr;
	}

	inFile.seekg(0, inFile.end);
	size_t size = inFile.tellg();
	inFile.seekg(0);

	char *buffer = new char[size];
	inFile.read(buffer, size);

	cout << "loaded data: " << size << endl;

	File *uPtr(new File(buffer, fileName, size));
	return  uPtr;
}

FileManager::FileManager()
{

}

File *FileManager::load(string filePath)
{
	if (!this->fileExist(filePath)) {
		throw new FileNotFoundException;
	}
	ifstream inFile(filePath.c_str(), ifstream::binary);
	string fileName = extractFileName(filePath);
	File *uPtr = extractData(inFile, fileName);
	inFile.close();

	return uPtr;

}

File *FileManager::load(string filePath, string fileName)
{
	string nameAndPath = filePath + "/" + fileName;
	//    ifstream inFile(nameAndPath.c_str(), ifstream::binary);
	//    unique_ptr<BinaryData> uPtr = extractData(inFile, fileName);
	//    inFile.close();

	return this->load(nameAndPath);
}

void FileManager::save(File *file)
{
	ofstream outFile(file->getFileName().c_str(), ofstream::binary);
	outFile.write(file->getData(), file->getSize());
	outFile.close();
	delete file;
}

bool FileManager::openOutput(string filePath)
{
	if (outFile.is_open()) {
		return false;
	}

	filePath = getValidFileName(filePath);

	outFile.open(filePath.c_str(), ofstream::binary | ofstream::app);
	if (outFile.is_open()) {
		return true;
	}
	return false;
}

void FileManager::closeOutput()
{
	if (outFile.is_open()) {
		outFile.close();
	}
}

bool FileManager::append(char *data, size_t size)
{
	if (!outFile.is_open()) {
		return false;
	}
	outFile.write(data, size);
	return true;
}

FileManager::~FileManager()
{
	if (outFile.is_open()) {
		outFile.close();
	}
}
