#include "statement.hpp"
#include "symbol_table.hpp"
#include <iostream>

// Implementacion de NoteStatement 
NoteStatement::NoteStatement(NoteExpression* note, DurationExpression* duration) noexcept
    : note{note}, duration{duration} {}

NoteExpression* NoteStatement::get_note() const noexcept {
    return note;
}

DurationExpression* NoteStatement::get_duration() const noexcept {
    return duration;
}

std::string NoteStatement::to_string() const noexcept {
    return note->to_string() + " " + duration->to_string();
}

void NoteStatement::destroy() noexcept {
    if (note != nullptr) {
        note->destroy();
        delete note;
    }
    
    if (duration != nullptr) {
        duration->destroy();
        delete duration;
    }
}

// Implementación del método resolve_names (verificacion semantica) para NoteStatement
bool NoteStatement::resolve_names(SymbolTable& table) noexcept{
    // Verificar que existan declaraciones necesarias antes de usar notas
    if (!table.contains("__tempo__"))
    {
        std::cerr << "Error: Es necesario declarar el tempo antes de usar notas.\n";
        return false;
    }
    
    if (!table.contains("__time_signature__"))
    {
        std::cerr << "Error: Es necesario declarar el compás antes de usar notas.\n";
        return false;
    }
    
    if (!table.contains("__key__"))
    {
        std::cerr << "Error: Es necesario declarar la tonalidad antes de usar notas.\n";
        return false;
    }
    
    // Validar la nota y la duración 
    if (!this->note->resolve_names(table))
    {
        return false;
    }
    
    if (!this->duration->resolve_names(table))
    {
        return false;
    }
    
    return true;
}

// Implementación de to_abc para NoteStatement
void NoteStatement::to_abc(std::ostream& out, double& beatCounter) const noexcept {
    // Escribir la nota en formato ABC
    out << note->as_abc() << duration->abc_suffix() << " ";
    
    // Actualizar el contador de beats
    beatCounter += duration->beats();
} 