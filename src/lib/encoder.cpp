#include "lib/encoder.h"

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

MorseL::Encoder::Encoder(Config cfg) : _cfg(cfg)
{}

MorseL::Encoder::~Encoder()
{}

/*string first(multimap<string, string> m, string key)
{
    auto r = m.equal_range(key);
    for (auto it = r.first; it != r.second; it++)
        return it->second;
    throw runtime_error("Clé non trouvé dans le dictionnaire");
}*/

string MorseL::Encoder::encode(const string& str)
{
    string ch = str;
    std::transform(ch.begin(), ch.end(), ch.begin(), [](unsigned char c){ return std::tolower(c); });

    //for (auto&& item : _cfg.data)
    //cout << item.first << ": " << item.second << '\n';

    string output = _cfg.data_e["begin"] + " / ";

    for (auto it = ch.cbegin() ; it != ch.cend(); ++it)
    {
        string its(1, *it);
        //cout << its << endl;
        if (_cfg.data_e.find(its) == _cfg.data_e.end())
        {
            if (its == " ")
            {
                output.append("/ ");
            }
            else
                throw runtime_error("Caractère " + its + " non trouvé !"); //TODO bonne exception
        } 
        else 
        {
            output.append(_cfg.data_e[its]);
            output.append(" ");
        }
        //std::cout << *it << ' ';
    }

    output.append("/ " + _cfg.data_e["end"]);

    output = Utils::string_replace(output, " / ", "/");

    return output;
}