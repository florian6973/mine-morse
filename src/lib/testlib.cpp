#include "lib/testlib.h"

#include <iostream>

using namespace std;

int MorseL::x = 3;

void MorseL::testlib()
{
    cout << "Bonjour de la librairie (x=" << MorseL::x << ")" << endl;
}