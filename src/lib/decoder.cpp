#include "lib/decoder.h"

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

MorseL::Decoder::Decoder(Config cfg) : _cfg(cfg)
{}

MorseL::Decoder::~Decoder()
{}

string MorseL::Decoder::decode(const string& str)
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
            output.append(_cfg.data_d[lettre]);
        }
        output.append(" ");
    }

    output = Utils::string_replace(output, "begin ", "");
    output = Utils::string_replace(output, " end", "");
    return output;
}