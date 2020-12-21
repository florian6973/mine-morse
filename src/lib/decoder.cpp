#include "lib/decoder.h"

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

string MorseL::Decoder::decode(Config& cfg, const string& str)
{
    string output = "";

    std::string delimiter = "/";
    std::string delimiter2 = " ";

    vector<string> mots = MorseL::Utils::split(str, delimiter);    
    for (const auto& mot: mots)
    {    
        vector<string> lettres = MorseL::Utils::split(mot, delimiter2);
        for (const auto& lettre:lettres)
        { 
            string tmp = cfg.data_d[lettre];
            if (tmp == "paragraphe")
                tmp = "\n";
            output.append(tmp);
        }
        output.append(" ");
    }

    output = Utils::string_replace(output, "begin ", "");
    output = Utils::string_replace(output, " end", "");
    return output;
}