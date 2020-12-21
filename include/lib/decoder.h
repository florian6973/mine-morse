#include "lib/config.h"
#include "lib/utils.h"

namespace MorseL
{
    class Decoder
    {
    public:
        static string decode(Config& cfg, const string& str);
    };
}