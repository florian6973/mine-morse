#include "console/main.h"

#include <iostream>
#include <fstream>

using namespace MorseC;

void MorseC::help()
{    
    cout << red << "Mine-Morse (Console)" << def << endl << endl;
    cout << "Utilisation : " << green << "./mine-morse_c <enc/dec> <input_file_or_text> [<output_file>]" << def << endl << endl;
    cout << "\t<enc/dec> : enc pour encoder, dec pour décoder" << endl;
    cout << "\t<input_file_or_text> : \n\t\tSi enc : chemin du fichier texte à encoder (ou texte directement)\n\t\tSi dec : chemin du fichier audio à décoder" << endl;
    cout << "\t[<output_file>] : \n\t\tSi enc : chemin du fichier audio de sortie\n\t\tSi dec : chemin du fichier texte de sortie contenant le message décodé (facultatif)" << endl;      
    cout << endl << "N. B. : Le message perd la casse à la traduction. Les fichiers sont écrasés s'ils existent déjà." << endl;
}

void MorseC::init_config(Config& conf, int argc, char** argv)
{
    conf.prog = string(argv[0]);
    
    string type = string(argv[1]);
    if (type == "enc")
        conf.enc = true;
    else if (type == "dec")
        conf.enc = false;
    else
        throw runtime_error("Paramètre '" + string(argv[1]) + "' inconnu !");

    conf.input = string(argv[2]);

    if (argc == 4)
        conf.output = string(argv[3]);

    conf.read("config.mm");

    cout << conf.prog << " : " << green << type << def << " de '" << green << conf.input << def << "' vers '" << green << conf.output << def << "'" << endl;
}

void MorseC::morse_trad(Config& conf)
{
    string str = "";

    if (conf.enc)
    {
        if (file_exists(conf.input))
            str = Encoder::encode_file(conf);
        else
            str = Encoder::encode_text(conf);

        Wav::write_file(conf, str);
        cout << "Fichier écrit : " << conf.output << endl;
    }
    else
    {
        str = Wav::read_file(conf);  
        cout << "Message lu : " << str << endl;
        str = Decoder::decode(conf, str);

        if (conf.output != "")
        {
            ofstream out(conf.output);
            out << str;
            out.close();
            
            cout << "Fichier écrit : " << conf.output << endl;
        }      
    }

    cout << "Résultat :\n\n" << green << str << def <<endl;
}

bool MorseC::file_exists(const string& name)
{
    return static_cast<bool>(ifstream(name));
}

int main(int argc, char** argv)
{
    cout << "------------------------------------------------------------------------------------------------------------" << endl;
    try
    {            
        Config conf;

        if ((argc < 3) || (argc > 4))
            help();
        else
        {
            init_config(conf, argc, argv);                       
            morse_trad(conf);
        }
    }
    catch(const exception& e)
    {
        cerr << endl << "\tERREUR : " << red << e.what() << def << endl;
    }

    cout << endl << "Fin du programme" << endl;
    cout << "------------------------------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------------------------------------------------------------------------------" << endl;
    return 0;
}