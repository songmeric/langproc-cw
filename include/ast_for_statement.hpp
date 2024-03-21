#ifndef AST_FOR_STATEMENT_HPP
#define AST_FOR_STATEMENT_HPP

#include "ast_node.hpp"

class ForStatement : public Node
{
private:
    Node *expression_statement_initialize_;
    Node *expression_statement_loop_;
    Node *statement_;

public:
    ForStatement(Node *expression_statement_initialize, Node *expression_statement_loop, Node *statement) : expression_statement_initialize_(expression_statement_initialize),
                                                                                                            expression_statement_loop_(expression_statement_loop),
                                                                                                            statement_(statement){};
    ~ForStatement()
    {
        delete expression_statement_initialize_;
        delete expression_statement_loop_;
        delete statement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

class ExprForStatement : public Node
{
private:
    Node *expression_statement_initialize_;
    Node *expression_statement_loop_;
    Node *expression_;
    Node *statement_;

public:
    ExprForStatement(Node *expression_statement_initialize, Node *expression_statement_loop, Node *expression, Node *statement) : expression_statement_initialize_(expression_statement_initialize),
                                                                                                                                expression_statement_loop_(expression_statement_loop),
                                                                                                                                expression_(expression),
                                                                                                                                statement_(statement){};
    ~ExprForStatement()
    {
        delete expression_statement_initialize_;
        delete expression_statement_loop_;
        delete expression_;
        delete statement_;
    };

    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
