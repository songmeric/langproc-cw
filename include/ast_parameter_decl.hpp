#ifndef AST_PARAMETER_DECL_HPP
#define AST_PARAMETER_DECL_HPP

#include "ast_node.hpp"

class ParameterDecl : public Node
{
private:
    Node *declaration_specifiers_;
    Node *declarator_;

public:
    ParameterDecl(Node *declaration_specifiers,
        Node *declarator)
        : declaration_specifiers_(declaration_specifiers)
        , declarator_(declarator){};
    ~ParameterDecl()
    {
        delete declaration_specifiers_;
        delete declarator_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
