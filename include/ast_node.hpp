#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <iostream>
#include <vector>

#include "ast_context.hpp"

class Node
{
protected:
    std::vector<Node *> branches_;

public:
    Node(){};
    virtual ~Node();
    virtual void EmitRISC(std::ostream &stream, Context &context) const = 0;
    virtual void Print(std::ostream &stream) const = 0;
};

// Represents a list of nodes.
class NodeList : public Node
{
private:
    std::vector<Node *> nodes_;

public:
    NodeList(Node *first_node) : nodes_({first_node}) {}

    ~NodeList()
    {
        for (auto node : nodes_)
        {
            delete node;
        }
    }

    Node *Child(size_t i) const
    {
        return nodes_.at(i);
    }

    Node *operator[](size_t i) const
    {
        return Child(i);
    }

    size_t size() const
    {
        return nodes_.size();
    }

    void PushBack(Node *item);
    virtual void EmitRISC(std::ostream &stream, Context &context) const override;
    virtual void Print(std::ostream &stream) const override;
};

enum Operator {
    OP_ASSIGN,

    // Basics
    OP_ADD,
    OP_ADDASSIGN,
    OP_SUB,
    OP_SUBASSIGN,
    OP_MUL,
    OP_MULASSIGN,
    OP_DIV,
    OP_DIVASSIGN,
    OP_MOD,
    OP_MODASSIGN,

    // Function call
    OP_CALL,

    // Indirect (->)
    OP_INDIRECT,

    // struct member (.)
    OP_MEMBER,

    // *
    OP_DEREFERENCE,

    // Shifts
    OP_LEFT,
    OP_LEFTASSIGN,
    OP_RIGHT,
    OP_RIGHTASSIGN,

    // Comparisons
    OP_CMPLT,
    OP_CMPLE,
    OP_CMPEQ,
    OP_CMPGE,
    OP_CMPGT,
    OP_CMPNE,

    // Comma
    OP_COMMA,

    // Bitwise
    OP_BITAND,
    OP_ANDASSIGN,
    OP_BITOR,
    OP_ORASSIGN,
    OP_BITXOR,
    OP_XORASSIGN,

    // Logical
    OP_LOGAND,
    OP_LOGOR,

    // Unary
    OP_INC,
    OP_DEC,
    OP_BITNOT,
    OP_LOGNOT,
    OP_ADDRESSOF,


};

#endif
