#pragma once

#include "ast_node_interface.hpp"
#include <string>
#include <vector>
#include <iostream>

// Enumeración para el modo de la tonalidad
enum class KeyMode {
    MAYOR, MENOR   
};

class Declaration : public ASTNodeInterface{
};

// Declaración de tempo
class TempoDeclaration : public Declaration{
public:
    TempoDeclaration(int tempo_value) noexcept;

    int get_tempo_value() const noexcept;
    std::string to_string() const noexcept override;
    void destroy() noexcept override;
    bool resolve_names(SymbolTable& table) noexcept override;
    void to_abc(std::ostream& out, double &beatCounter) const noexcept override;

private:
    int tempo_value;
};

// Declaración de compás
class TimeSignatureDeclaration : public Declaration{
public:
    TimeSignatureDeclaration(int numerator, int denominator) noexcept;

    int get_numerator() const noexcept;
    int get_denominator() const noexcept;
    std::string to_string() const noexcept override;
    void destroy() noexcept override;
    bool resolve_names(SymbolTable& table) noexcept override;
    void to_abc(std::ostream& out, double &beatCounter) const noexcept override;

private:
    int numerator;
    int denominator;
};

// Declaración de clave (tonalidad)
class KeyDeclaration : public Declaration{
public:
    KeyDeclaration(const std::string& note, KeyMode mode) noexcept;

    std::string get_note() const noexcept;
    KeyMode get_mode() const noexcept;
    std::string to_string() const noexcept override;
    void destroy() noexcept override;
    bool resolve_names(SymbolTable& table) noexcept override;
    void to_abc(std::ostream& out, double &beatCounter) const noexcept override;

private:
    std::string note;
    KeyMode mode;
};

// Forward declaration de Statement
class Statement;

// Clase que representa el nodo raíz del AST
class MusicProgram : public ASTNodeInterface{
public:
    MusicProgram() noexcept;
    ~MusicProgram() noexcept;

    // Añadir declaraciones y statements al programa
    void add_declaration(Declaration* declaration) noexcept;
    void add_statement(Statement* statement) noexcept;

    // Acceso a las declaraciones y statements
    const std::vector<Declaration*>& get_declarations() const noexcept;
    const std::vector<Statement*>& get_statements() const noexcept;

    // Métodos de la interfaz ASTNodeInterface
    std::string to_string() const noexcept override;
    void destroy() noexcept override;
    bool resolve_names(SymbolTable& table) noexcept override;
    void to_abc(std::ostream& out, double &beatCounter) const noexcept override;

private:
    std::vector<Declaration*> declarations;
    std::vector<Statement*> statements;
};

// Estructura auxiliar para pasar las tres declaraciones juntas
struct Encabezado {
    TempoDeclaration* tempo;
    TimeSignatureDeclaration* compas;
    KeyDeclaration* key;
}; 