#include "lib/utils.h"

string MorseL::Utils::get_path(const string& base, const string& file)
{
    return string_replace(string_replace(string_replace(base, "bin/", "data/"), "./", "../data/"), "mine-morse_c", file);
}

string MorseL::Utils::string_replace(string src, string const& target, string const& repl)
{
    if (target.length() == 0) 
        return src;

    if (src.length() == 0)
        return src;

    size_t idx = 0;

    for (;;)
    {
        idx = src.find(target, idx);
        if (idx == string::npos)
            break;

        src.replace(idx, target.length(), repl);
        idx += repl.length();
    }

    return src;
}

vector<string> MorseL::Utils::split(const string& src, const string& delim)
{
    string ch = src;
    vector<string> exit = {};

    size_t pos = 0;
    std::string token;
    while ((pos = ch.find(delim)) != std::string::npos)
    {
        token = ch.substr(0, pos);
        exit.push_back(token);
        ch.erase(0, pos + delim.length());
    }
    exit.push_back(ch);

    return exit;
}
