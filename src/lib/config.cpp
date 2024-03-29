#include "lib/config.h"
#include "lib/utils.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <exception>
#include <iterator>

MorseL::Config::Config()
{}

MorseL::Config::~Config()
{}

void MorseL::Config::read(string file)
{
    string _file = Utils::get_path(prog, file);

    ifstream cFile (_file);

    if (cFile.is_open())
    {
        string line;

        while(getline(cFile, line))
        {
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

            if(line[0] == '#' || line.empty())
                continue;

            string delimiter = "¤";
            auto delimiterPos = line.find(delimiter);
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + delimiter.length());

            if (name.rfind("wav.", 0) == 0)
                wav[name] = stoi(value);
            else
            {
                if (wav["wav.debug"])
                    cout << name << " = " << value << " ; "; 

                data_e[name] = value;
                data_d[value] = name;
            }            
        }
        if (wav["wav.debug"])
            cout << endl;

        cFile.close();
    }
    else
    {
        throw runtime_error("Impossible d'ouvrir le fichier de configuration '" + _file + "' !");
    }
}