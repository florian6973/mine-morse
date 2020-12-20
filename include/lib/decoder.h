#include "lib/config.h"
#include "lib/utils.h"

#include <memory>

namespace MorseL
{
    class Decoder
    {
    public:
        Decoder(Config cfg);
        ~Decoder();

        string decode(const string& str);
    private:
        Config _cfg;
    };
}