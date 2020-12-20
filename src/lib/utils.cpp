#include "lib/utils.h"

string MorseL::Utils::get_path(const string& base, const string& file)
{
    return string_replace(string_replace(string_replace(base, "bin/", "data/"), "./", "../data/"), "mine-morse_c", file);
}

string MorseL::Utils::string_replace( string src, string const& target, string const& repl)
{
    // handle error situations/trivial cases

    if (target.length() == 0) {
        // searching for a match to the empty string will result in 
        //  an infinite loop
        //  it might make sense to throw an exception for this case
        return src;
    }

    if (src.length() == 0) {
        return src;  // nothing to match against
    }

    size_t idx = 0;

    for (;;) {
        idx = src.find( target, idx);
        if (idx == string::npos)  break;

        src.replace( idx, target.length(), repl);
        idx += repl.length();
    }

    return src;
}