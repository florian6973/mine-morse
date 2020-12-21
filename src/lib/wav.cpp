#include "lib/wav.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace MorseL;

template <typename Word>
std::ostream& MorseL::Wav::write_word(std::ostream& outs, Word value, unsigned size)
{
    char* a_begin = reinterpret_cast<char*>(&value);
    char* a_end = a_begin + size;

    vector<char> chars;
    copy(a_begin, a_end, back_inserter(chars));

    for (long unsigned int i = 0; i < chars.size(); i++)
      outs.write(&chars[i], sizeof(char));

    return outs;
}

void MorseL::Wav::write_motif(ostream& f, const bool& vide, const double& seconds, const WavInfo& wvi)
{
    int N = wvi.hz * seconds;
    for (int n = 0; n < N; n++)
    {
        if (vide)
            write_word(f, 0, wvi.nbbytes);
        else
        {
            double amplitude = (double)n / N * wvi.max_ampl;
            double value = sin((two_pi * n * wvi.freq) / wvi.hz);
            write_word(f, (int)(amplitude  * value), wvi.nbbytes);
        }
    }
}

void MorseL::Wav::write_file(Config& cfg, const string& msg)
{
    string fpath = cfg.output;
    if (fpath == "")
        throw runtime_error("Un fichier de sortie (.wav) doit être spécifié !");
    
    std::fstream f;
    f.open(fpath, ios::in | ios::out | ios::binary | ios:: trunc);

    WavInfo wvi;
    wvi.nbbytes = cfg.wav["wav.bps_o"];
    wvi.hz = cfg.wav["wav.hz"];
    int bps = wvi.nbbytes*8;
    int nbchan = 1; // seulement une channel prévue (pas stéréo)
    wvi.freq = cfg.wav["wav.freq"];
      
    f << "RIFF----WAVEfmt "; // (chunk size to be filled in later)
    write_word(f, 16, 4);
    write_word(f, 1, 2);
    write_word(f, nbchan, 2);
    write_word(f, wvi.hz, 4);
    write_word(f, wvi.hz*nbchan*bps/8, 4);  
    write_word(f, nbchan*bps/8, 2);
    write_word(f, bps, 2);

    size_t data_chunk_pos = f.tellp();
    f << "data----";  // (chunk size to be filled in later) 

    double d_ti = 60.0/((double)cfg.wav["wav.wpm"]*50.0); // convention mot PARIS pour WPM
    wvi.max_ampl = pow(2, bps - 1);  // "volume"

    bool wordprec = false; // pour savoir s'il faut mettre un espace inter-lettres
    for(long unsigned int i = 0; i<msg.length(); i++)
    {
        switch (msg.at(i))
        {
            case '.':
                if (wordprec)
                    write_motif(f, true, cfg.wav["wav.espaceinter"]*d_ti, wvi);

                write_motif(f, false, cfg.wav["wav.ti"]*d_ti, wvi);
                wordprec = true;
                break;
            case '-':
                if (wordprec)
                    write_motif(f, true, cfg.wav["wav.espaceinter"]*d_ti, wvi);

                write_motif(f, false, cfg.wav["wav.ta"]*d_ti, wvi);
                wordprec = true;
                break;
            case ' ':
                write_motif(f, true, cfg.wav["wav.espacelettre"]*d_ti, wvi);
                wordprec = false;
                break;
            case '/':
                write_motif(f, true, cfg.wav["wav.espacemot"]*d_ti, wvi);
                wordprec = false;
                break;
            default:
                throw runtime_error("Code inconnu !");
        }
    }
  
    // Fix the data chunk header to contain the data size
    size_t file_length = f.tellp();
    f.seekp(data_chunk_pos + 4);
    write_word(f, (file_length - data_chunk_pos + 8), 4);
    f.seekp(4);
    write_word(f, (file_length - 8), 4); 

    f.close();
}

template <typename T>
T MorseL::Wav::extract(const vector<unsigned char> &v, int pos)
{
    T value;
    memcpy(&value, &v[pos], sizeof(T));
    return value;
}

int MorseL::Wav::read_int(const vector<unsigned char>& vec, int pos, const int size)
{
    vector<unsigned char> buffer;
    for (int i = pos; i < (pos+size); i++)
      buffer.push_back(vec[i]);
    int a = 0;

    switch (size)
    {
      case 1:
        a = extract<int8_t>(buffer, 0);
        break;
      case 2:
        a = extract<short>(buffer, 0);
        break;
      case 4:
        a = extract<int>(buffer, 0);
        break;
      default:
        throw runtime_error("Taille de bloc non prise en compte");
    }

    return a;
}

vector<BYTE> MorseL::Wav::readFile(const string& filename)
{
    std::streampos fileSize;
    std::ifstream file(filename, ios::binary | ios::ate);

    file.seekg(0, ios::end);
    fileSize = file.tellg();
    file.seekg(0, ios::beg);

    vector<BYTE> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}

string MorseL::Wav::read_file(Config& cfg)
{
    string fpath = cfg.input;
    
    vector<unsigned char> result = readFile(fpath);

    //int fsize = read_int(result, 4, 4);
    //int audformat = read_int(result, 20, 2);
    int nbrcan = read_int(result, 22, 2);
    int freq = read_int(result, 24, 4);
    //int bpsec = read_int(result, 28, 4);
    //int bpp = read_int(result, 32, 2);
    int bpsample = read_int(result, 34, 2);
    //int datasize = read_int(result, 40, 4);
    
    if (nbrcan > 1)
      throw runtime_error("Nombre de canaux non compatible");

    int delta = 44; // taille du header
    int incr = bpsample/8; // taille d'un sample en octets
    double duration = (double)result.size()/(double)incr/(double)freq; // durée totale du fichier audio
    int max_amplitude = (int)pow(2, bpsample - 1); 

    vector<vector<double>> tzero = {}; // stocke les vecteurs de la forme { durée du silence, temps à la fin du silence }
    double ti = -1; // temps d'un ti (détecté par minimum)

    int nbz = 0; // nombre de zéros/max à la suite
    bool write = false; // variable mémoire pour savoir si le nombre de zéros/max est significatif
    int nbt = 10; // critère pour définir quand une série de zéros/max est significative
    for (long unsigned int i = delta; i < result.size(); i+=incr)
    {
        int z = read_int(result, i, incr);
        double tps = (double)nbz/(double)freq; // Temps du silence actuel

        if (tps > (double)nbt/freq)
            write = true;

        if ((z == 0) || (z == -(max_amplitude))) // selon les conventions des fichiers audios
            nbz++;
        else
        {
            nbz = 0;
            if (write)
            {
                double curtps = (double)i/(double)incr/(double)freq; // Current time
                if ((ti < 0) || (tps < ti)) // Recherche du minimum
                    ti = tps;       

                tzero.push_back({tps, curtps });
                write = false;
            }
        }
    }

    if (write) // ne pas oublier le dernier silence, s'il est significatif
    {          
        double tps = (double)nbz/(double)freq;      
        double curtps = (double)result.size()/(double)incr/(double)freq;
        tzero.push_back({ tps, curtps }); 
    }

    vector<int> morse = {}; // contient les caractères morses sous forme d'un entier
    double tlast = 0; // temps de la fin du dernier silence
    for (long unsigned int i = 0; i < tzero.size(); i++)
    {
        morse.push_back(round((tzero[i][1]-tzero[i][0]-tlast)/ti)); // ajout lettre
        morse.push_back(-round((tzero[i][0])/ti)); // ajout silence

        tlast = tzero[i][1];
    }
    morse.push_back(round((duration-tlast)/ti)); // ajout dernière lettre, au besoin

    string output = "";
    for (long unsigned int i = 0; i < morse.size(); i++)
    {
        if (cfg.wav["wav.debug"])
            cout << morse[i] << " ";

        switch (morse[i])
        {
            case 1:
            output.append(".");
            break;

            case 3:
            output.append("-");
            break;

            case -3:
            case -13: // https://www.meridianoutpost.com/resources/etools/calculators/calculator-morse-code.php
            output.append(" ");
            break;

            case -7:
            case -9: // https://www.radio-amater.rs/morsecodegenerator/
            case -38: // https://www.meridianoutpost.com/resources/etools/calculators/calculator-morse-code.php
            output.append("/");
            break;
        }
    }

    if (cfg.wav["wav.debug"])
        cout << endl;

    return output;
}