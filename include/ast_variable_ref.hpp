#ifndef AST_VARIABLE_REF_HPP
#define AST_VARIABLE_REF_HPP

#include "ast_node.hpp"
#include "ast_identifier.hpp"

class VariableReference : public Node
{
private:
    Identifier *variable_identifier_;

public:
    VariableReference(Identifier *variable_identifier)
        : variable_identifier_(variable_identifier)
    {
    }
    ~VariableReference()
    {
        delete variable_identifier_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
