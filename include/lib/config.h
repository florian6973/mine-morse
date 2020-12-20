#include <string>
#include <map>

using namespace std;

namespace MorseL
{
    class Config
    {
        public:
            Config(char* exec, string file);
            ~Config();
            void read();
            void save();

            map<string, string> data = {};
        private:
            string _file = "";
    };
}