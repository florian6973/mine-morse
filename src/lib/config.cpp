#include "lib/config.h"
#include "lib/utils.h"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <exception>

MorseL::Config::Config(char* exec, string file) : _file(Utils::get_path(string(exec), file))
{ cout << "Fichier de configuration : " << _file << endl;}

void MorseL::Config::read()
{
    ifstream cFile (_file);

    if (cFile.is_open())
    {
        string line;

        while(getline(cFile, line))
        {
            line.erase(remove_if(line.begin(), line.end(), ::isspace),
                                 line.end());

            if(line[0] == '#' || line.empty())
                continue;

            string delimiter = "¤";
            auto delimiterPos = line.find(delimiter);
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + delimiter.length());

            //cout << name << " " << value << endl;

            data["name"] = value;
        }

        cFile.close();
    }
    else
    {
        throw runtime_error("Impossible d'ouvrir le fichier '" + _file + "' !");
    }
}

void MorseL::Config::save()
{    
    throw runtime_error("Non implémenté !");
}

MorseL::Config::~Config()
{}