#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

// Estructura que representa un símbolo en la tabla
struct Symbol{
    std::string name;
    
    static std::shared_ptr<Symbol> build(std::string_view name) noexcept;
};

// Clase que implementa la tabla de símbolos
class SymbolTable{
public:
    using TableType = std::unordered_map<std::string, std::shared_ptr<Symbol>>;
    using TableStack = std::vector<TableType>;

    SymbolTable() noexcept;
    ~SymbolTable() noexcept;

    // Gestión de ámbitos
    void enter_scope() noexcept;
    bool exit_scope() noexcept;
    TableType::size_type scope_level() const noexcept;

    // Métodos principales
    bool insert(const std::string& name) noexcept;
    bool contains(const std::string& name) noexcept;
    std::shared_ptr<Symbol> lookup(const std::string& name) noexcept;
    std::shared_ptr<Symbol> current_scope_lookup(const std::string& name) noexcept;

private:
    static std::shared_ptr<Symbol> find_in_scope(const std::string& name, const TableType& scope) noexcept;
    TableStack scopes;
}; 