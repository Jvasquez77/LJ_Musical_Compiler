#include <iostream>
#include <fstream>
#include <string>
#include "declaration.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include "symbol_table.hpp"
#include "ast_node_interface.hpp"

// Declaraciones externas para el scanner y parser
extern FILE* yyin;
extern int yyparse();
extern MusicProgram* parser_result;

void mostrar_ayuda() {
    std::cerr << "Uso: ./compilador_musical <archivo_entrada.mus> [archivo_salida.abc]\n";
    std::cerr << "Si no se especifica archivo de salida, se usará el mismo nombre con extensión .abc\n";
}

int main(int argc, char* argv[]) {
    // Verificar argumentos de línea de comandos
    if (argc < 2 || argc > 3) {
        mostrar_ayuda();
        return 1;
    }

    std::string archivo_entrada = argv[1];
    
    // Verificar que el archivo tenga la extensión .mus
    if (archivo_entrada.substr(archivo_entrada.size() - 4) != ".mus") {
        std::cerr << "Error: El archivo de entrada debe tener extensión .mus\n";
        return 1;
    }
    
    // Determinar el nombre del archivo de salida
    std::string archivo_salida;
    if (argc == 3) {
        archivo_salida = argv[2];
    } else {
        archivo_salida = archivo_entrada.substr(0, archivo_entrada.size() - 4) + ".abc";
    }
    
    // Abrir el archivo de entrada
    FILE* entrada = fopen(archivo_entrada.c_str(), "r");
    if (!entrada) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo_entrada << "\n";
        return 1;
    }
    
    // Configurar el scanner para leer desde el archivo
    yyin = entrada;
    
    std::cout << "Analizando archivo: " << archivo_entrada << "...\n";
    
    // Fase 1: Construcción del AST
    int resultado_parse = yyparse();
    fclose(entrada);
    
    if (resultado_parse != 0 || parser_result == nullptr) {
        std::cerr << "Error: Falló la construcción del AST.\n";
        return 1;
    }
    
    std::cout << "Construcción del AST exitosa.\n";
    
    // Fase 2: Análisis semántico
    SymbolTable tabla_simbolos;
    bool analisis_semantico_ok = parser_result->resolve_names(tabla_simbolos);
    
    if (!analisis_semantico_ok) {
        std::cerr << "Error: Falló el análisis semántico.\n";
        return 1;
    }
    
    std::cout << "Análisis semántico exitoso.\n";
    
    // Fase 3: Generación de código ABC
    std::ofstream archivo_abc(archivo_salida);
    if (!archivo_abc) {
        std::cerr << "Error: No se pudo crear el archivo de salida " << archivo_salida << "\n";
        return 1;
    }
    
    double contador_beats = 0.0;
    parser_result->to_abc(archivo_abc, contador_beats);
    
    archivo_abc.close();
    std::cout << "Traducción exitosa. Se generó el archivo: " << archivo_salida << "\n";
    
    // Fase 4: Liberar memoria
    parser_result->destroy();
    delete parser_result;
    
    return 0;
} 