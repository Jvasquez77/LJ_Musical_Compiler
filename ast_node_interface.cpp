#include "ast_node_interface.hpp"
#include "statement.hpp"

void destroy_program_body(ProgramBody& body) noexcept{
    while (!body.empty())
    {
        Statement* stmt = body.front();
        body.pop_front();
        
        if (stmt != nullptr)
        {
            stmt->destroy();
            delete stmt;
        }
    }
} 