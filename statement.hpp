#pragma once

#include "ast_node_interface.hpp"
#include "expression.hpp"
#include <string>

class Statement : public ASTNodeInterface{
    // Clase base para los statements
};

class NoteStatement : public Statement{
public:
    NoteStatement(NoteExpression* note, DurationExpression* duration) noexcept;

    NoteExpression* get_note() const noexcept;
    DurationExpression* get_duration() const noexcept;
    std::string to_string() const noexcept override;
    void destroy() noexcept override;
    bool resolve_names(SymbolTable& table) noexcept override;
    void to_abc(std::ostream& out, double &beatCounter) const noexcept override;

private:
    NoteExpression* note;
    DurationExpression* duration;
}; 