#include "lib/config.h"
#include "lib/encoder.h"
#include "lib/decoder.h"
#include "lib/utils.h"
#include "lib/wav.h"

using namespace std;
using namespace MorseL;
using namespace MorseL::Utils;

namespace MorseC
{
    void help();
    void init_config(Config& conf, int argc, char** argv);
    void morse_trad(Config& conf);
    bool file_exists(const std::string& name);
    
    Color::Modifier red(Color::FG_RED);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);    
}

int main(int argc, char** argv);
