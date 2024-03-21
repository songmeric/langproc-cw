#ifndef AST_WHILE_STATEMENT_HPP
#define AST_WHILE_STATEMENT_HPP

#include "ast_node.hpp"

class WhileStatement : public Node
{
private:
    Node *expression_;
    Node *statement_;

public:
    WhileStatement(Node *expression, Node *statement) : expression_(expression), statement_(statement){};
    ~WhileStatement()
    {
        delete expression_;
        delete statement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

class DoWhileStatement : public Node
{
private:
    Node *statement_;
    Node *expression_;

public:
    DoWhileStatement(Node *statement, Node *expression) : statement_(statement), expression_(expression){};
    ~DoWhileStatement()
    {
        delete statement_;
        delete expression_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
