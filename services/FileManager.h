#include <memory>
#include <string>
#include <fstream>
#include "../models/File.h"
#include "../lib/FileNotFoundException.h"
#include <sys/stat.h>

using namespace std;
class FileManager
{
	ofstream outFile;
	bool fileExist(string filePath);
	string getValidFileName(string fileName);
	string extractFileName(string filePath);
	File *extractData(ifstream &inFile, string fileName);
public:
	FileManager();
	File *load(string filePath);
	File *load(string filePath, string fileName);
	void save(File *file);
	bool openOutput(string filePath);
	void closeOutput();
	bool append(char *data, size_t size);

	~FileManager();
	// usuniecie konstruktora kopiujacego i operatora przypisania
	FileManager(FileManager const&) = delete;
	FileManager& operator=(FileManager const&) = delete;
};