#include "lib/config.h"
#include "lib/utils.h"

#include <memory>

namespace MorseL
{
    class Encoder
    {
    public:
        Encoder(Config cfg);
        ~Encoder();

        string encode(const string& str);
    private:
        Config _cfg;
    };
}