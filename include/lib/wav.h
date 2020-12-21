#include "lib/config.h"
#include "lib/utils.h"

namespace MorseL
{ 
    struct WavInfo
    {
        int hz;
        int nbbytes;
        int freq;
        double max_ampl;
    };
 
    typedef unsigned char BYTE;

    class Wav
    {
    public:
        static void write_file(Config& cfg, const string& msg);
        static string read_file(Config& cfg);
    private:
        static void write_motif(ostream& f, const bool& vide, const double& seconds, const WavInfo& wvi);
        template <typename Word>
        static std::ostream& write_word(std::ostream& outs, Word value, unsigned size = sizeof(Word));

        template <typename T>
        static T extract(const vector<unsigned char> &v, int pos);
        static int read_int(const vector<unsigned char>& vec, int r, const int size);
        static vector<BYTE> readFile(const string& filename);

        static constexpr double two_pi = 6.283185307179586476925286766559;
    };
}