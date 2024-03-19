#ifndef AST_IF_STATEMENT_HPP
#define AST_IF_STATEMENT_HPP

#include "ast_node.hpp"

class IfStatement : public Node
{
private:
    Node *expression_;
    Node *statement_;

public:
    IfStatement(Node *expression, Node *statement) : expression_(expression), statement_(statement){};
    ~IfStatement()
    {
        delete expression_;
        delete statement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

class IfElseStatement : public Node
{
private:
    Node *expression_;
    Node *statement_;
    Node *elsestatement_;

public:
    IfElseStatement(Node *expression, Node *statement, Node *elsestatement) : expression_(expression), statement_(statement), elsestatement_(elsestatement){};
    ~IfElseStatement()
    {
        delete expression_;
        delete statement_;
        delete elsestatement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
