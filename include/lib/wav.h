#include "lib/utils.h"

namespace MorseL
{
    class Wav
    {
    public:
        static void writefile(char* exec, const string& filepath, const string& msg, const int& wpm=20);
        static string readfile(char* exec, const string& filepath);
    };
}

// TODO problème accent