#ifndef AST_COMPOUND_STMT_HPP
#define AST_COMPOUND_STMT_HPP

#include "ast_node.hpp"

class CompoundStatement : public Node
{
private:
    Node *decls_;
    Node *stmts_;

public:
    CompoundStatement(Node *decls, Node *stmts)
        : decls_(decls)
        , stmts_(stmts) {}
    ~CompoundStatement()
    {
        delete decls_;
        delete stmts_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
