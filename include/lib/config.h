#pragma once //compiled only once

#include <string>
#include <map>

using namespace std;

namespace MorseL
{
    class Config
    {
        public:
            Config();
            ~Config();
            void read(string file);

            map<string, string> data_e = {};
            map<string, string> data_d = {};

            bool enc = false;
            string prog = "";
            string input = "";
            string output = "";
    };
}