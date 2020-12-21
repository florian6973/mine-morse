#pragma once

#include <ostream>
#include <string>
#include <vector>

using namespace std;

namespace MorseL::Color
{
    enum Code
    {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };

    class Modifier
    {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}

        friend std::ostream& operator<<(std::ostream& os, const Modifier& mod)
        {
            return os << "\033[" << mod.code << "m";
        }
    };

}

namespace MorseL::Utils
{    
    string string_replace(string src, string const& target, string const& repl);

    string get_path(const string& base, const string& file);

    vector<string> split(const string& src, const string& delim);
}