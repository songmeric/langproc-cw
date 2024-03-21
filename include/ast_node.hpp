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
    // Basics
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,

    // Function call
    OP_CALL,

    // Indirect (->)
    OP_INDIRECT,
    OP_MEMBER,

    // Shifts
    OP_LEFT,
    OP_RIGHT,

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
    OP_BITOR,
    OP_BITXOR,

    // Logical
    OP_LOGAND,
    OP_LOGOR,

    // Unary
    OP_INC,
    OP_DEC,
    OP_BITNOT,
    OP_LOGNOT,
    OP_ADDRESSOF
};

#endif
