// rscpp.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "ui\UserInterface.h"
using namespace std;

int main(int argc, char **argv) {
	UserInterface *ui = new UserInterface();
	if (argc == 5) {
		ui->init(argv[1], argv[2], argv[3], argv[4]);
	}
	else {
		ui->init();
	}
	 delete ui;

	return 0;
}

