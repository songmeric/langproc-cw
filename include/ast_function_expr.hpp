#ifndef AST_FUNCTION_EXPR_CPP
#define AST_FUNCTION_EXPR_CPP

#include "ast_node.hpp"

class FunctionExpression : public Node
{
private:
    Node *fn_;

public:
    FunctionExpression(Node *fn) : fn_(fn){};
    ~FunctionExpression(){};
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
