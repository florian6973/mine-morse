#include "console/main.h"

#include <iostream>

Color::Modifier red(Color::FG_RED);
Color::Modifier def(Color::FG_DEFAULT);

int main(int argc, char** argv)
{
    try
    {    
        Config conf(argv[0], "config.mm"); // Unix dependent , TODO tester sous windows !
        conf.read();

        //msg to lower
    }
    catch(const exception& e)
    {
        cerr << endl << "\tERREUR : " << red << e.what() << def << endl;
    }

    return 0;
}