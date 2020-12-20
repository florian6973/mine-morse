#include "lib/utils.h"

namespace MorseL
{
    class Wav
    {
    public:
        static void writefile(char* exec, const string& filepath);
        static void readfile(char* exec, const string& filepath);
    };
}