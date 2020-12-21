#include "lib/encoder.h"

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

string MorseL::Encoder::encode_file(Config& cfg)
{
    ifstream f_in(cfg.input);
    cfg.input = string((istreambuf_iterator<char>(f_in)), istreambuf_iterator<char>());    
    return encode_text(cfg);
}

string MorseL::Encoder::encode_text(Config& cfg)
{     
    string ch = cfg.input;    
    std::transform(ch.begin(), ch.end(), ch.begin(), [](unsigned char c){ return std::tolower(c); });

    string output = cfg.data_e["begin"] + " / ";

    for (auto it = ch.cbegin() ; it != ch.cend(); ++it)
    {
        string its(1, *it);

        if (cfg.data_e.find(its) == cfg.data_e.end())
        {
            if (its == " ")
                output.append("/ ");
            else if (its == "\n")
                output.append(cfg.data_e["paragraphe"] + " ");
            else
                throw runtime_error("Caractère '" + its + "' non trouvé !"); //TODO bonne exception
        } 
        else 
        {
            output.append(cfg.data_e[its]);
            output.append(" ");
        }
    }

    output.append("/ " + cfg.data_e["end"]);
    output = Utils::string_replace(output, " / ", "/");

    return output;
}