#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    std::string abc_file = "../test.abc";
    std::string midi_file = "output.mid";
    std::string ps_file = "output.ps";
    std::string pdf_file = "output.pdf";
    std::string ogg_file = "output.ogg";

    // ABC EN PROTOCOLOS MIDI
    std::string cmd1 = "abc2midi " + abc_file + " -o " + midi_file;
    if (std::system(cmd1.c_str()) != 0) {
        std::cerr << "Error de interfaz (no se pudo convertir midi)\n";
        return 1;
    }

    // Convertir ABC a PostScript
    std::string cmd3 = "abcm2ps " + abc_file + " -O " + ps_file;
    if (std::system(cmd3.c_str()) != 0) {
        std::cerr << "Error al convertir ABC a PostScript\n";
        return 1;
    }

    // Convertir PostScript a PDF
    std::string cmd4 = "ps2pdf " + ps_file + " " + pdf_file;
    if (std::system(cmd4.c_str()) != 0) {
        std::cerr << "Error al convertir PostScript a PDF\n";
        return 1;
    }
    
    // Convertir MIDI a OGG usando timidity
    std::string cmd2 = "timidity " + midi_file + " -Ow -o " + ogg_file;  //-0w salida en formato ogg y -o nombre del archivo de salida
    if (std::system(cmd2.c_str()) != 0) {
        std::cerr << "Error al convertir MIDI a OGG\n";
        return 1;
    }

    std::cout << "Conversión completada.\n";
    return 0;
}