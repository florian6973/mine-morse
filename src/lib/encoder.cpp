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
    if (cfg.wav["wav.debug"])
        cout << cfg.input << " -> " << ch << endl;

    string output = cfg.data_e["begin"] + " / ";

    for (size_t i = 0; i < ch.length();) // gérer l'UTF-8
    {
        string its = "";

        int cplen = 1;
        if ((ch[i] & 0xf8) == 0xf0) 
            cplen = 4;
        else if ((ch[i] & 0xf0) == 0xe0) 
            cplen = 3;
        else if ((ch[i] & 0xe0) == 0xc0)
            cplen = 2;            
        if ((i + cplen) > ch.length())
            cplen = 1;

        its = ch.substr(i, cplen);
        i += cplen;

        if (cfg.wav["wav.debug"])
            cout << its << " ";

        if (cfg.data_e.find(its) == cfg.data_e.end())
        {
            if (its == " ")
                output.append("/ ");
            else if (its == "\n")
                output.append(cfg.data_e["paragraphe"] + " ");
            else
                throw runtime_error("Caractère '" + its + "' non trouvé !");
        } 
        else 
        {
            output.append(cfg.data_e[its]);
            output.append(" ");
        }
    }
    if (cfg.wav["wav.debug"])
        cout << endl;

    output.append("/ " + cfg.data_e["end"]);
    output = Utils::string_replace(output, " / ", "/");

    return output;
}