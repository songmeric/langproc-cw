#ifndef AST_SWITCH_STATEMENT_HPP
#define AST_SWITCH_STATEMENT_HPP

#include "ast_node.hpp"

class SwitchStatement : public Node
{
private:
    Node *expression_;
    Node *statement_;

public:
    SwitchStatement(Node *expression, Node *statement) : expression_(expression), statement_(statement){};
    ~SwitchStatement()
    {
        delete expression_;
        delete statement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

class CaseStatement : public Node
{
private:
    Node *constant_expression_;
    Node *statement_;

public:
    CaseStatement(Node *constant_expression, Node *statement) : constant_expression_(constant_expression), statement_(statement){};
    ~CaseStatement()
    {
        delete constant_expression_;
        delete statement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

class DefaultStatement : public Node
{
private:
    Node *statement_;

public:
    DefaultStatement(Node *statement) : statement_(statement){};
    ~DefaultStatement()
    {
        delete statement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif

