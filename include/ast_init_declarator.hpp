#ifndef AST_INIT_DECLARATOR_HPP
#define AST_INIT_DECLARATOR_HPP

#include "ast_node.hpp"

class InitDeclarator : public Node
{
private:
    Node *identifier_;
    Node *value_;

public:
    InitDeclarator(Node *identifier, Node *value)
        : identifier_(identifier)
        , value_(value) {};
    ~InitDeclarator()
    {
        delete identifier_;
        delete value_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
