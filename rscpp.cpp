// rscpp.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"
#include <thread>
#include <iostream>

using namespace std;

void aaa() {
	cout << "asdada\n";
}
int main()
{
	thread t(aaa);
	t.join();
	system("pause");
    return 0;
}

