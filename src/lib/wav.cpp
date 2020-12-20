#include "lib/wav.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

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

    vector<ushort> data_b = {0x5249, 0x4646,
                                0xFFFF,0xFFFF,
                               0x5741,0x5645,
                              0x666D, 0x7420,
                               0x1000,0x0000,
                               0x0100,
                               0x0100,
                               11025,
                               11025,
                               1,
                               8,
                               0x6461,0x7461,
                               0xFFFF,0xFFFF
                               };
// End of RAII block. This will close the stream.
    std::fstream file;
    //uint64_t myuint = 0xFFFF;
    file.open(path, ios::in | ios::out | ios::binary | ios:: trunc);

    /*for (auto b:data_b)
    {   
        file.write(reinterpret_cast<char*>(&b), sizeof(b));
    }*/


  double hz        = 11025;    // samples per second
  double frequency = 441;  // middle C
  double seconds   = 2.5;      // time

  int N = hz * seconds;

    int h_size = 44;
    int d_size = N*8; // 8 car BitsPerSample

    //problème de big endian et little endian
    string hex = "52494646" + int_to_hex(h_size + d_size) + "57415645666D742010000000010001002B1100002B1100000100080064617461" + int_to_hex(d_size);
std::basic_string<uint8_t> bytes;

// Iterate over every pair of hex values in the input string (e.g. "18", "0f", ...)
for (size_t i = 0; i < hex.length(); i += 2)
{
    uint16_t byte;

    // Get current pair and store in nextbyte
    std::string nextbyte = hex.substr(i, 2);

    // Put the pair into an istringstream and stream it through std::hex for
    // conversion into an integer value.
    // This will calculate the byte value of your string-represented hex value.
    std::istringstream(nextbyte) >> std::hex >> byte;

    // As the stream above does not work with uint8 directly,
    // we have to cast it now.
    // As every pair can have a maximum value of "ff",
    // which is "11111111" (8 bits), we will not lose any information during this cast.
    // This line adds the current byte value to our final byte "array".
    bytes.push_back(static_cast<uint8_t>(byte));
}

// we are now generating a string obj from our bytes-"array"
// this string object contains the non-human-readable binary byte values
// therefore, simply reading it would yield a String like ".0n..:j..}p...?*8...3..x"
// however, this is very useful to output it directly into a binary file like shown below
std::string result(begin(bytes), end(bytes));


    file << result;

    constexpr double two_pi = 6.283185307179586476925286766559;
  constexpr double max_amplitude = 32760;  // "volume"
  // total number of samples
  for (int n = 0; n < N; n++)
  {
    double amplitude = (double)n / N * max_amplitude;
    double value     = sin( (two_pi * n * frequency) / hz );
    write_word( file, (int)(                 amplitude  * value), 2 );
    //write_word( file, (int)((max_amplitude - amplitude) * value), 2 );
  }


    //assert(file.is_open());

    //for(int i = 0; i < sizeof(array) / sizeof(array[0]); ++i)
    //   file.write((char*)(array + i * sizeof(array[0])), sizeof(array[0]));
    //file.write(reinterpret_cast<char*>(&myuint), sizeof(myuint));
  f.seekp( 40);
  write_word( f, file_length - data_chunk_pos + 8 );

  // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    f.seekp( 4 );
    write_word( f, file_length - 8, 4 ); 
    file.close();

    cout << "fichier écrit " << path << endl;

    cout << int_to_hex(560) << endl;
}