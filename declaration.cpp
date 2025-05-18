#include "declaration.hpp"
#include "statement.hpp"
#include "symbol_table.hpp"
#include <vector>
#include <iostream>
#include <cmath>

// TempoDeclaration implementacion
TempoDeclaration::TempoDeclaration(int tempo_value) noexcept
    : tempo_value{tempo_value} {}

int TempoDeclaration::get_tempo_value() const noexcept {
    return tempo_value;
}

std::string TempoDeclaration::to_string() const noexcept {
    return "Tempo " + std::to_string(tempo_value);
}

void TempoDeclaration::destroy() noexcept {
    // No hay memoria que liberar
}

// Implementación del método resolve_names para TempoDeclaration
bool TempoDeclaration::resolve_names(SymbolTable& table) noexcept{
    if (table.contains("__tempo__"))
    {
        std::cerr << "Error: Tempo declarado más de una vez.\n";
        return false;
    }
    
    if (tempo_value < 20 || tempo_value > 200){
        std::cerr << "Error: El tempo debe estar en el rango de Larghissimo a Prestissimo.\n";
        return false;
    }
    
    table.insert("__tempo__");
    return true;
}

// Implementación de to_abc para TempoDeclaration
void TempoDeclaration::to_abc(std::ostream& out, double& /*beatCounter*/) const noexcept {
    out << "Q:1/4=" << tempo_value << "\n";
}

// implementacion de la declaracion de compas
TimeSignatureDeclaration::TimeSignatureDeclaration(int numerator, int denominator) noexcept
    : numerator{numerator}, denominator{denominator} {}

int TimeSignatureDeclaration::get_numerator() const noexcept {
    return numerator;
}

int TimeSignatureDeclaration::get_denominator() const noexcept {
    return denominator;
}

std::string TimeSignatureDeclaration::to_string() const noexcept {
    return "Compas " + std::to_string(numerator) + "/" + std::to_string(denominator);
}

void TimeSignatureDeclaration::destroy() noexcept {
    // No hay memoria que liberar
}

// Implementación del método resolve_names para TimeSignatureDeclaration
bool TimeSignatureDeclaration::resolve_names(SymbolTable& table) noexcept{
    if (table.contains("__time_signature__"))
    {
        std::cerr << "Error: Compás declarado más de una vez.\n";
        return false;
    }
    
    if (numerator <= 1 || numerator > 12){
        std::cerr << "Error: El numerador del compás debe ser mayor a 1 y menor a 12.\n";
        return false;
    }
    
    if (denominator != 2 && denominator != 4 && denominator != 8 && denominator != 16)
    {
        std::cerr << "Error: El denominador del compás debe ser 2, 4, 8 o 16.\n";
        return false;
    }
    
    table.insert("__time_signature__");
    return true;
}

// Implementación de to_abc para TimeSignatureDeclaration
void TimeSignatureDeclaration::to_abc(std::ostream& out, double& /*beatCounter*/) const noexcept {
    out << "M:" << numerator << "/" << denominator << "\n";
    out << "L:1/8\n"; // Establecer la unidad básica como corchea
}

// implementacion de la declaracion de clave (tonalidad)
KeyDeclaration::KeyDeclaration(const std::string& note, KeyMode mode) noexcept
    : note{note}, mode{mode} {}

std::string KeyDeclaration::get_note() const noexcept {
    return note;
}

KeyMode KeyDeclaration::get_mode() const noexcept {
    return mode;
}

std::string KeyDeclaration::to_string() const noexcept {
    std::string mode_str = (mode == KeyMode::MAYOR) ? "M" : "m";
    return "Tonalidad " + note + " " + mode_str;
}

void KeyDeclaration::destroy() noexcept {
}

// Implementación del método resolve_names(verificacion semantica) para KeyDeclaration
bool KeyDeclaration::resolve_names(SymbolTable& table) noexcept{
    if (table.contains("__key__"))
    {
        std::cerr << "Error: Tonalidad declarada más de una vez.\n";
        return false;
    }
    
    // Verificar que la nota raíz sea válida
    // Lista de notas musicales válidas
    std::vector<std::string> valid_notes = {"Do", "Re", "Mi", "Fa", "Sol", "La", "Si", 
                                          "Do#", "Re#", "Fa#", "Sol#", "La#", "Si#",
                                          "Dob", "Reb", "Mib", "Solb", "Lab", "Sib",
                                          "C", "D", "E", "F", "G", "A", "B",
                                          "Cb", "Db", "Eb", "Gb", "Ab", "Bb",
                                          "C#", "D#", "F#", "G#", "A#", "B#"
                                          };
    
    bool valid_root = false;
    for (const auto& n : valid_notes)
    {
        if (note == n)
        {
            valid_root = true;
            break;
        }
    }
    
    if (!valid_root)
    {
        std::cerr << "Error: Nota raíz inválida: " << note << ".\n";
        return false;
    }
    
    table.insert("__key__");
    return true;
}

// Implementación de to_abc para KeyDeclaration
void KeyDeclaration::to_abc(std::ostream& out, double& /*beatCounter*/) const noexcept {
    // Convertir la nota base de una tonalidad a formato ABC
    std::string abc_note;
    if (note == "Do" || note == "C") abc_note = "C";
    else if (note == "Re" || note == "D") abc_note = "D";
    else if (note == "Mi" || note == "E") abc_note = "E";
    else if (note == "Fa" || note == "F") abc_note = "F";
    else if (note == "Sol" || note == "G") abc_note = "G";
    else if (note == "La" || note == "A") abc_note = "A";
    else if (note == "Si" || note == "B") abc_note = "B";
    
    // Manejar sostenidos y bemoles
    if (note.find('#') != std::string::npos || note.find("is") != std::string::npos) {
        abc_note += "maj"; // Mayor con sostenido
    } else if (note.find('b') != std::string::npos || note.find("es") != std::string::npos) {
        abc_note += "min"; // Menor con bemol
    } else {
        // Determinar el modo
        abc_note += (mode == KeyMode::MAYOR) ? "maj" : "min";
    }
    
    out << "K:" << abc_note << "\n";
}

// Implementación de la clase MusicProgram configuracion musical
MusicProgram::MusicProgram() noexcept{
}

MusicProgram::~MusicProgram() noexcept{
    this->destroy();
}

void MusicProgram::add_declaration(Declaration* declaration) noexcept{
    if (declaration != nullptr)
    {
        this->declarations.push_back(declaration);
    }
}

void MusicProgram::add_statement(Statement* statement) noexcept{
    if (statement != nullptr)
    {
        this->statements.push_back(statement);
    }
}

const std::vector<Declaration*>& MusicProgram::get_declarations() const noexcept{
    return this->declarations;
}

const std::vector<Statement*>& MusicProgram::get_statements() const noexcept{
    return this->statements;
}

std::string MusicProgram::to_string() const noexcept{
    std::string result = "Programa musical:\n";

    // Imprimir todas las declaraciones
    result += "Declaraciones:\n";
    for (const auto& decl : this->declarations)
    {
        result += "  " + decl->to_string() + "\n";
    }

    // Imprimir todos los statements
    result += "Sentencias:\n";
    for (const auto& stmt : this->statements)
    {
        result += "  " + stmt->to_string() + "\n";
    }

    return result;
}

void MusicProgram::destroy() noexcept{ 
    // Destruir todas las declaraciones
    for (auto& decl : this->declarations)
    {
        if (decl != nullptr)
        {
            decl->destroy();
            delete decl;
        }
    }
    this->declarations.clear();

    // Destruir todos los statements
    for (auto& stmt : this->statements)
    {
        if (stmt != nullptr)
        {
            stmt->destroy();
            delete stmt;
        }
    }
    this->statements.clear();
}

bool MusicProgram::resolve_names(SymbolTable& table) noexcept{
    // Primero procesar todas las declaraciones
    for (const auto& decl : this->declarations)
    {
        if (!decl->resolve_names(table))
        {
            return false;
        }
    }

    // Luego procesar todos los statements
    for (const auto& stmt : this->statements)
    {
        if (!stmt->resolve_names(table))
        {
            return false;
        }
    }

    // Verificar que las declaraciones obligatorias existan
    if (!table.contains("__tempo__"))
    {
        std::cerr << "Error: Falta declaración de tempo.\n";
        return false;
    }

    if (!table.contains("__time_signature__"))
    {
        std::cerr << "Error: Falta declaración de compás.\n";
        return false;
    }

    if (!table.contains("__key__"))
    {
        std::cerr << "Error: Falta declaración de tonalidad.\n";
        return false;
    }

    return true;
}

// Implementación de to_abc para MusicProgram
void MusicProgram::to_abc(std::ostream& out, double& beatCounter) const noexcept {
    // Cabecera mínima ABC
    out << "X:1\n";
    out << "T:LJ CERTIFIED TRANSLATION\n";
    
    // Procesar todas las declaraciones primero
    for (const auto& decl : declarations) {
        decl->to_abc(out, beatCounter);
    }
    
    // Procesar todas las notas
    for (const auto& stmt : statements) {
        stmt->to_abc(out, beatCounter);
        
        // Insertar barra de compás cuando se completa un compás
        if (std::fmod(beatCounter, 7.0) == 0.0) {
            out << "| ";
        }
    }
    
    // Finalizar la partitura con una barra final
    out << "|\n";
} 