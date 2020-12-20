#include "console/main.h"

#include <iostream>

Color::Modifier red(Color::FG_RED);
Color::Modifier def(Color::FG_DEFAULT);

//make clean sinon compile mal parfois

int main(int argc, char** argv)
{
    try
    {    
        Config conf(argv[0], "config.mm"); // Unix dependent , TODO tester sous windows !
        conf.read();

        if (argc >= 2)
        {
            Encoder ecd(conf);
            //cout << "test";
            string en = ecd.encode(string(argv[1]));
            //cout << "testb";
            cout << en << endl;

            Decoder dcd(conf);
            string de = dcd.decode(en);
            cout << de << endl; //new line = new paragraphe !

            //overwrite
            Wav::writefile(argv[0], "test.wav");
            Wav::readfile(argv[0], "test.wav");
        }
        else
        {
            cout << "please enter" << endl;
        }
        //msg to lower
    }
    catch(const exception& e)
    {
        cerr << endl << "\tERREUR : " << red << e.what() << def << endl;
    }

    return 0;
}