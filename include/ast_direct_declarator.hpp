#ifndef AST_DIRECT_DECLARATOR_HPP
#define AST_DIRECT_DECLARATOR_HPP

#include "ast_node.hpp"

class DirectDeclarator : public Node
{
private:
    Node *identifier_;
    Node *parameters_;

public:
    DirectDeclarator(Node *identifier, Node *parameters)
    : identifier_(identifier), parameters_(parameters) {}
    ~DirectDeclarator()
    {
        delete identifier_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
