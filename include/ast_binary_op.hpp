#ifndef AST_BINARY_OP_HPP
#define AST_BINARY_OP_HPP

#include "ast_node.hpp"

class BinaryOp : public Node
{
private:
    Node *lhs_;
    Operator oper_;
    Node *rhs_;

public:
    BinaryOp(Node *lhs, Operator oper, Node *rhs)
        : lhs_(lhs), oper_(oper), rhs_(rhs){}
    ~BinaryOp()
    {
        delete lhs_;
        delete rhs_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
