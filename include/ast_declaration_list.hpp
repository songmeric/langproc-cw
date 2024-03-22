#ifndef AST_DECLARATION_LIST_HPP
#define AST_DECLARATION_LIST_HPP

#include "ast_node.hpp"

class DeclarationList : public Node
{
private:
    Node *type_;
    Node *list_;

public:
    DeclarationList(Node *type, Node *list)
        : type_(type), list_(list)
    {
        std::cerr << "Constructing DeclarationList with"
            " type=";
        type->Print(std::cerr);
        std::cerr << " list=";
        list->Print(std::cerr);
        std::cerr << "\n";
    }
    ~DeclarationList()
    {
        delete type_;
        delete list_;
    };
    void EmitRISC(std::ostream &stream, Context &context) const override;
    void Print(std::ostream &stream) const override;
};

#endif
