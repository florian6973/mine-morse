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

using namespace std;

template< typename T >
std::string int_to_hex( T i )
{
  std::stringstream stream;
  stream << "" 
         << std::setfill ('0') << std::setw(sizeof(T)*2) 
         << std::hex << i;
  return stream.str();
}

namespace little_endian_io
{
  template <typename Word>
  std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
  {
    for (; size; --size, value >>= 8)
      outs.put( static_cast <char> (value & 0xFF) );
    return outs;
  }
}
using namespace little_endian_io;


void write(ostream& f, bool vide, double seconds, int hz, int bps)
{
  constexpr double two_pi = 6.283185307179586476925286766559;
  double max_amplitude = pow(2,bps - 1 );  // "volume"
  double frequency = 400*2;  // A

  int N = hz * seconds;  // total number of samples
  for (int n = 0; n < N; n++)
  {
    if (vide)
      write_word( f, 0, bps/8 ); // problème of size
    else
    {
      double amplitude = (double)n / N * max_amplitude;
      double value     = sin( (two_pi * n * frequency) / hz );
      write_word( f, (int)(amplitude  * value), bps/8 ); // problème of size
    }
    //write_word( f, (int)((max_amplitude - amplitude) * value), 2 );

    //write_word( f, (int)max_amplitude-1, bps/8 ); // problème of size
  }
}

void MorseL::Wav::writefile(char* exec, const string& filepath, const string& msg, const int& wpm)
{
    string path = Utils::get_path(string(exec), filepath); // à changer ancien
    
    //unsigned short array[2]={ox20ac,0x20bc};
// End of RAII block. This will close the stream.
    std::fstream f;
    //uint64_t myuint = 0xFFFF;
    f.open(path, ios::in | ios::out | ios::binary | ios:: trunc);

   
  int hz        = 44100; //baisser qualité ?
  int bps = 16; // multiple de 8
  int nbchan = 1;
  f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
  write_word( f,     16, 4 );  // no extension data
  write_word( f,      1, 2 );  // PCM - integer samples
  write_word( f,      nbchan, 2 );  // two channels (stereo file)
  write_word( f,  hz, 4 );  // samples per second (Hz)
  write_word( f, hz*nbchan*bps/8, 4 );  
  write_word( f,      nbchan*bps/8, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
  write_word( f,     bps, 2 );  // number of bits per sample (use a multiple of 8)

  // Write the data chunk header
  size_t data_chunk_pos = f.tellp();
  f << "data----";  // (chunk size to be filled in later)
  
  // Write the audio samples
  // (We'll generate a single C4 note with a sine wave, fading from left to right)
   // samples per second

   double d_ti = 60.0/((double)wpm*50.0); //mot PARIS et non CODEX (60) //pourquoi 1200 ?
  //cout << d_ti << endl;
bool wordprec = false;
 for(long unsigned int i = 0; i<msg.length(); i++) {
      //cout << msg.at(i) << endl; //get character at position i
      //cout << msg.at(i);
      switch (msg.at(i))
      {
        case '.':
          if (wordprec)
            write(f, true, d_ti, hz, bps);
          write(f, false, d_ti, hz, bps);
          wordprec = true;
          break;
        case '-':
          if (wordprec)
            write(f, true, d_ti, hz, bps);
          write(f, false, 3*d_ti, hz, bps);
          wordprec = true;
          break;
        case ' ':
          write(f, true, 3*d_ti, hz, bps);
          wordprec = false;
          break;
        case '/':
          write(f, true, 7*d_ti, hz, bps);
          wordprec = false;
          break;
        default:
          throw runtime_error("Code inconnu !");
      }
   }


  
  // (We'll need the final file size to fix the chunk sizes above)
  size_t file_length = f.tellp();

  // Fix the data chunk header to contain the data size
  f.seekp( data_chunk_pos + 4 );
  write_word( f, /*N*nbchan*bps-44*/ (file_length - data_chunk_pos + 8), 4 );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
  f.seekp( 0 + 4 );
  write_word( f,  /*N*nbchan*bps-8*/(file_length - 8), 4 ); 


    f.close();

    cout << (file_length - 8) << endl;
    cout << (file_length - data_chunk_pos + 8) << endl;
    cout << "fichier écrit " << path << endl;
}

/*void endian_swap(unsigned int& x)
{
    x = (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}*/

/*int buffToInteger(vector<char> buffer)
{
    int a = static_cast<int>(static_cast<unsigned char>(buffer[0]) << 24 |
        static_cast<unsigned char>(buffer[1]) << 16 | 
        static_cast<unsigned char>(buffer[2]) << 8 | 
        static_cast<unsigned char>(buffer[3]));
    return a;
}*/

int read_int(const vector<char>& vec, int r, const int size)
{
    vector<char> fsize;
    for (int i = r; i < (r+size); i++)
      fsize.push_back(vec[i]);
    int a = 0;
    memcpy( &a, fsize.data(), sizeof( int ) );
    return a;
}

/*int read_int(const vector<char>& vec, int r, const int size)
{
    vector<char> buffer;
    for (int i = r; i < (r+size); i++)
      buffer.push_back(vec[i]);
    int a = 0;
    if (size == 4)
    {
    a = static_cast<int>(static_cast<unsigned char>(buffer[0]) << 24 |
        static_cast<unsigned char>(buffer[1]) << 16 | 
        static_cast<unsigned char>(buffer[2]) << 8 | 
        static_cast<unsigned char>(buffer[3]));
    }
    else if (size == 2)
    {
    a = static_cast<int>(static_cast<unsigned char>(buffer[0]) << 8 | 
        static_cast<unsigned char>(buffer[1]));
    }
    return a;
}*/

string MorseL::Wav::readfile(char* exec, const string& filepath)
{
    string path = Utils::get_path(string(exec), filepath); // à changer ancien
    
    //unsigned short array[2]={ox20ac,0x20bc};
// End of RAII block. This will close the stream.
    std::ifstream inFile;
    //uint64_t myuint = 0xFFFF;
    inFile.open(path, std::ios::in|ios::binary|ios::ate); // sans ate segmentation fault ?
    inFile.unsetf(std::ios::skipws);
if (!inFile)
{
  throw runtime_error("Ne peut pas lire");
}
    inFile.seekg(0, std::ios::end);
    ifstream::pos_type pos = inFile.tellg();
    std::vector<char>  result(pos);

    //std::vector<unsigned char> result(std::istreambuf_iterator<char>(inFile), {});
    inFile.seekg(0, ios::beg);
    inFile.read(&result[0], pos);
    if (inFile)
      std::cout << "all characters read successfully.";
    else
      std::cout << "error: only " << inFile.gcount() << " could be read"  ;


    inFile.close();
    //for (int i = 0; i < 8; i++)
    //  cout << result[i];
    //cout << endl;

    int fsize = read_int(result, 4, 4);
    int audformat = read_int(result, 20, 2);
    int nbrcan = read_int(result, 22, 2);
    int freq = read_int(result, 24, 4);
    int bpsec = read_int(result, 28, 4);
    int bpp = read_int(result, 32, 2);
    int bpsample = read_int(result, 34, 2);
    int datasize = read_int(result, 40, 4);
    cout << fsize << " " << audformat << " " << nbrcan << " " << freq << " " << freq << " " << bpsec << " " << bpp << " " << bpsample << " " << datasize << endl;
    
    //chercher temps entre 0...
    //commence à 44
    int delta = 44;
    int nbz = 0;
    bool write = false;
    int incr = bpsample/8;

    double duration = (double)result.size()/(double)incr/(double)freq;

    vector<vector<double>> tzero = {};

    for (int i = delta; i < result.size(); i+=incr)
    {
        int z = read_int(result, i, 2);

        double tps = (double)nbz/(double)freq;

        if (tps > (double)2/(freq)) // deux périodes sinon critère de Shannon !
        { 
          write = true;
        }

        if (z==0)
        {
          nbz++;
        }
        else
        {
          nbz=0;//800 hz : période 1/800*
          if (write)
          {
            double curtps = (double)i/(double)incr/(double)freq;
            //cout << tps << endl;
            //cout << curtps << endl;
            //cout << endl;
            tzero.push_back({tps, curtps }); //bug avec pair ??
            write = false;
          }
        }
    }
    //tzero.push_back({0, (double)result.size()/(double)incr/(double)freq }); 
    //le dernier mettre des zéros ?    

    //cout << tzero.size() << endl;

    double ti = (*std::min_element(begin(tzero), end(tzero), [](auto lhs, auto rhs) {
  return lhs[0] < rhs[0];
}))[0];
    cout << "base duration" << ti << endl;
    //cout << ti << endl;

    vector<int> morse = {};
    double tlast = 0;
    for (int i = 0; i < tzero.size(); i++)
    {
      morse.push_back(round((tzero[i][1]-tzero[i][0]-tlast)/ti));
      morse.push_back(-round((tzero[i][0])/ti));
      tlast = tzero[i][1];
    }
    //ajout dernière lettre
    morse.push_back(round((duration-tlast)/ti));

    string output = "";
    for (int i = 0; i < morse.size(); i++)
    {
      switch (morse[i])
      {
          case 1:
          output.append(".");
          break;
          case 3:
          output.append("-");
          break;
          case -3:
          output.append(" ");
          break;
          case -7:
          output.append("/");
          break;
      }
    }
    cout << output << endl;
    return output;
    //cout << endian_swap(buffToInteger(fsize)) << endl;
}