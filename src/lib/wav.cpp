#include "lib/wav.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>

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

void MorseL::Wav::writefile(char* exec, const string& filepath)
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
  constexpr double two_pi = 6.283185307179586476925286766559;
  double max_amplitude = pow(2,bps - 1 );  // "volume"
   // samples per second
  double frequency = 400*2;  // A
  double seconds   = 2;      // time

  int N = hz * seconds;  // total number of samples
  for (int n = 0; n < N; n++)
  {
    double amplitude = (double)n / N * max_amplitude;
    double value     = sin( (two_pi * n * frequency) / hz );
    write_word( f, (int)(                 amplitude  * value), bps/8 ); // problème of size
    //write_word( f, (int)((max_amplitude - amplitude) * value), 2 );

    //write_word( f, (int)max_amplitude-1, bps/8 ); // problème of size
  }
  N = hz * seconds;  // total number of samplesotal number of samples
  for (int n = 0; n < N; n++)
  {
    //double amplitude = (double)n / N * max_amplitude;
    //double value     = sin( (two_pi * n * frequency) / hz );
    write_word( f, 0, bps/8 ); // problème of size
    //write_word( f, (int)((max_amplitude - amplitude) * value), 2 );
  }
  N = hz * seconds;  // total number of samples
  for (int n = 0; n < N; n++)
  {
    double amplitude = (double)n / N * max_amplitude;
    double value     = sin( (two_pi * n * frequency) / hz );
    write_word( f, (int)(                 amplitude  * value), bps/8 ); // problème of size
    //write_word( f, (int)((max_amplitude - amplitude) * value), 2 );
    //write_word( f, (int)max_amplitude-1, bps/8 ); // problème of size
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

    cout << "fichier écrit " << path << endl;
    cout << (file_length - 8) << endl;
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

void MorseL::Wav::readfile(char* exec, const string& filepath)
{
    string path = Utils::get_path(string(exec), filepath); // à changer ancien
    
    //unsigned short array[2]={ox20ac,0x20bc};
// End of RAII block. This will close the stream.
    std::ifstream inFile;
    //uint64_t myuint = 0xFFFF;
    inFile.open(path, ios::binary|ios::ate); // sans ate segmentation fault ?
    cout << "test" << endl;

    ifstream::pos_type pos = inFile.tellg();
    std::vector<char>  result(pos);
    cout << "test" << endl;

    inFile.seekg(0, ios::beg);
    inFile.read(&result[0], pos);
    if (inFile)
      std::cout << "all characters read successfully.";
    else
      std::cout << "error: only " << inFile.gcount() << " could be read"  ;

    cout << "test" << endl;

    inFile.close();
    for (int i = 0; i < 8; i++)
      cout << result[i];
    cout << endl;
    vector<char> fsize;
    for (int i = 4; i < 8; i++)
      fsize.push_back(result[i]);
    int a = 0;
    memcpy( &a, fsize.data(), sizeof( int ) );

    cout << a << endl;
    //cout << endian_swap(buffToInteger(fsize)) << endl;
}