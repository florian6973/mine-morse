#include "lib/config.h"
#include "lib/utils.h"

#include <memory>

namespace MorseL
{
    class Encoder
    {
    public:
        static string encode_file(Config& cfg);
        static string encode_text(Config& cfg);
    };
}