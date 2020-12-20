#pragma once //compiled only once

#include <string>
#include <map>
#include <iterator> 

using namespace std;

namespace MorseL
{
    class Config
    {
        public:
            Config(); //for default member class
            Config(char* exec, string file);
            ~Config();
            void read();
            void save();

            map<string, string> data_e = {};
            map<string, string> data_d = {};
        private:
            string _file = "";
    };
}