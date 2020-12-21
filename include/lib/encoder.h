#include "lib/config.h"
#include "lib/utils.h"

namespace MorseL
{
    class Encoder
    {
    public:
        static string encode_file(Config& cfg);
        static string encode_text(Config& cfg);
    };
}