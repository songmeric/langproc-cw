#ifndef AST_ARGUMENT_EXPR_CPP
#define AST_ARGUMENT_EXPR_CPP

#include "ast_node.hpp"

class ArgumentExpression : public Node
{
private:
    Node *arg_;

public:
    ArgumentExpression(Node *arg) : arg_(arg){};
    ~ArgumentExpression(){};
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
