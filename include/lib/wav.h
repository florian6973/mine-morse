#include "lib/config.h"
#include "lib/utils.h"

namespace MorseL
{
    class Wav
    {
    public:
        static void write_file(Config& cfg, const string& msg);
        static string read_file(Config& cfg);
    };
}

// TODO problème accent