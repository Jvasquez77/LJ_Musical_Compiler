#include "expression.hpp"
#include "symbol_table.hpp"
#include <iostream>
#include <vector>
#include <cctype>

// implementacion de NoteExpression 
NoteExpression::NoteExpression(const std::string& note_name, int octave) noexcept
    : note_name{note_name}, octave{octave} {}

std::string NoteExpression::get_note_name() const noexcept {
    return note_name;
}

int NoteExpression::get_octave() const noexcept {
    return octave;
}

std::string NoteExpression::to_string() const noexcept {
    return note_name + std::to_string(octave);
}

void NoteExpression::destroy() noexcept {
    // No hay memoria que liberar
}

// implementacion de DurationExpression 
DurationExpression::DurationExpression(DurationType type) noexcept
    : duration_type{type} {}

DurationType DurationExpression::get_duration_type() const noexcept {
    return duration_type;
}

std::string DurationExpression::to_string() const noexcept {
    switch (duration_type) {
        case DurationType::SEMICORCHEA: return "Semicorchea";
        case DurationType::CORCHEA: return "Corchea";
        case DurationType::NEGRA: return "Negra";
        case DurationType::BLANCA: return "Blanca";
        default: return "Unknown";
    }
}

void DurationExpression::destroy() noexcept {
 
}

// implementacion del metodo resolve_names (verificacion semantica) para NoteExpression
bool NoteExpression::resolve_names(SymbolTable& table) noexcept{
    // Verificar que la nota sea válida
    std::vector<std::string> valid_notes = {"Do", "Re", "Mi", "Fa", "Sol", "La", "Si", 
                                          "Do#", "Re#", "Fa#", "Sol#", "La#", "Si#",
                                          "Dob", "Reb", "Mib", "Solb", "Lab", "Sib",
                                          "C", "D", "E", "F", "G", "A", "B",
                                          "Cb", "Db", "Eb", "Gb", "Ab", "Bb",
                                          "C#", "D#", "F#", "G#", "A#", "B#"
                                          };
    
    bool valid_note = false;
    for (const auto& note : valid_notes)
    {
        if (note_name == note)
        {
            valid_note = true;
            break;
        }
    }
    
    if (!valid_note)
    {
        std::cerr << "Error: Nota inválida: " << note_name << ".\n";
        return false;
    }
    
    // Verificar que la octava esté en un rango válido (1-8)
    if (octave < 1 || octave > 8)
    {
        std::cerr << "Error: Octava fuera de rango (1-8): " << octave << ".\n";
        return false;
    }
    
    return true;
}

// implementacion del metodo resolve_names (verificacion semantica) para DurationExpression
bool DurationExpression::resolve_names(SymbolTable& /*table*/) noexcept{
    // Todas las duraciones son válidas porque están definidas como enum
    return true;
}

// Implementación de to_abc para NoteExpression
void NoteExpression::to_abc(std::ostream& /*out*/, double& /*beatCounter*/) const noexcept {
    //se usa as_abc()
}

// Implementación del método auxiliar as_abc() para NoteExpression
std::string NoteExpression::as_abc() const noexcept {
    std::string abc_note;
    std::string base_note = note_name;
    bool has_sharp = false;
    bool has_flat = false;
    
    // Detectar sostenidos y bemoles
    if (note_name.find('#') != std::string::npos || note_name.find("is") != std::string::npos) {
        has_sharp = true;
        // Eliminar el sostenido del nombre para obtener la nota base
        if (note_name.find('#') != std::string::npos) {
            base_note = note_name.substr(0, note_name.find('#'));
        } else if (note_name.find("is") != std::string::npos) {
            base_note = note_name.substr(0, note_name.find("is"));
        }
    } else if (note_name.find('b') != std::string::npos || note_name.find("es") != std::string::npos) {
        has_flat = true;
        // Eliminar el bemol del nombre para obtener la nota base
        if (note_name.find('b') != std::string::npos) {
            base_note = note_name.substr(0, note_name.find('b'));
        } else if (note_name.find("es") != std::string::npos) {
            base_note = note_name.substr(0, note_name.find("es"));
        }
    }
    
    // Convertir nombre de notas individuales a notación ABC
    if (base_note == "Do" || base_note == "C") abc_note = "C";
    else if (base_note == "Re" || base_note == "D") abc_note = "D";
    else if (base_note == "Mi" || base_note == "E") abc_note = "E";
    else if (base_note == "Fa" || base_note == "F") abc_note = "F";
    else if (base_note == "Sol" || base_note == "G") abc_note = "G";
    else if (base_note == "La" || base_note == "A") abc_note = "A";
    else if (base_note == "Si" || base_note == "B") abc_note = "B";
    
    // Añadir sostenido o bemol si es necesario
    if (has_sharp) {
        abc_note = "^" + abc_note; // ^ para sostenido en notación ABC
    } else if (has_flat) {
        abc_note = "_" + abc_note; // _ para bemol en notación ABC
    }
    
    // Ajustar la octava
    if (octave <= 3) {
        // Notas bajas (C,, para octava 2, C, para octava 3)
        for (int i = 0; i < 4 - octave; ++i) {
            abc_note += ",";
        }
    } else if (octave >= 5) {
        // Notas altas (c para octava 5, c' para octava 6, etc.)
        // Convertir primera letra a minúscula
        char firstChar = std::tolower(abc_note[0]);
        abc_note = firstChar + abc_note.substr(1);
        
        for (int i = 0; i < octave - 5; ++i) {
            abc_note += "'";
        }
    }
    
    return abc_note;
}

// Implementación de to_abc para DurationExpression
void DurationExpression::to_abc(std::ostream& /*out*/, double& /*beatCounter*/) const noexcept {
    // se usa abc_suffix() y beats()
}

// Implementación del método auxiliar abc_suffix() para DurationExpression
std::string DurationExpression::abc_suffix() const noexcept {
    switch (duration_type) {
        case DurationType::SEMICORCHEA: return "/";
        case DurationType::CORCHEA: return "";
        case DurationType::NEGRA: return "2";
        case DurationType::BLANCA: return "4";
        default: return "";
    }
}

// Implementación del método auxiliar beats() para DurationExpression
double DurationExpression::beats() const noexcept {
    switch (duration_type) {
        case DurationType::SEMICORCHEA: return 0.5;
        case DurationType::CORCHEA: return 1.0;
        case DurationType::NEGRA: return 2.0;
        case DurationType::BLANCA: return 4.0;
        default: return 1.0;
    }
} 