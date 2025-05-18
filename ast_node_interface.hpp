#pragma once

#include <string>
#include <string_view>
#include <forward_list>
#include <ostream>


class Declaration;
class Expression;
class Statement;
class SymbolTable;

// Definimos la interfaz base para todos los nodos del AST
class ASTNodeInterface{
public:

    virtual ~ASTNodeInterface() noexcept = default;
    
    virtual void destroy() noexcept = 0;
    
    // Método para representación en string 
    virtual std::string to_string() const noexcept = 0;

    // Método para verificacion en el análisis semántico
    virtual bool resolve_names(SymbolTable& table) noexcept = 0;
    
    // Método para generar notación ABC
    virtual void to_abc(std::ostream& out,
                        double &beatCounter) const noexcept = 0;
};
// Alias útil para representar un cuerpo de instrucciones
using ProgramBody = std::forward_list<Statement*>;

// Función auxiliar para destruir todos los elementos en un cuerpo
void destroy_program_body(ProgramBody& body) noexcept; 