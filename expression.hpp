#pragma once

#include "ast_node_interface.hpp"
#include <string>

// Enumeración para el tipo de duración
enum class DurationType {
    SEMICORCHEA,   // Semicorchea
    CORCHEA,       // Corchea
    NEGRA,         // Negra
    BLANCA         // Blanca
};

class Expression : public ASTNodeInterface{
};

class NoteExpression : public Expression{
public:
    NoteExpression(const std::string& note_name, int octave) noexcept;

    std::string get_note_name() const noexcept;
    int get_octave() const noexcept;
    std::string to_string() const noexcept override;
    void destroy() noexcept override;
    bool resolve_names(SymbolTable& table) noexcept override;
    void to_abc(std::ostream& out, double &beatCounter) const noexcept override;
    
    // Método auxiliar para obtener la nota en formato ABC
    std::string as_abc() const noexcept;

private:
    std::string note_name;
    int octave;
};

class DurationExpression : public Expression{
public:
    DurationExpression(DurationType type) noexcept;

    DurationType get_duration_type() const noexcept;
    std::string to_string() const noexcept override;
    void destroy() noexcept override;
    bool resolve_names(SymbolTable& table) noexcept override;
    void to_abc(std::ostream& out, double &beatCounter) const noexcept override;
    
    // Métodos auxiliares para notación ABC
    std::string abc_suffix() const noexcept;
    double beats() const noexcept;

private:
    DurationType duration_type;
}; 