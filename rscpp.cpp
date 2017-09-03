// rscpp.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "ui\UserInterface.h"
using namespace std;

int main() {
	UserInterface *ui = new UserInterface();
	 ui->init();
	 delete ui;

	return 0;
}

