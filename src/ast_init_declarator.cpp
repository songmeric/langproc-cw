#include <sstream>
#include "ast_init_declarator.hpp"

void InitDeclarator::EmitRISC(std::ostream &stream, Context &context) const
{
    std::ostringstream ss;

    identifier_->Print(ss);
    std::string name = ss.str();
    Variable *v = context.DeclareVariable(context.declarationListType, name);

    value_->EmitRISC(stream, context);
    stream << "addi sp,sp,-4\n";
    stream << "sw a0," << (long)v->offset << "(s0)\n";
}

void InitDeclarator::Print(std::ostream &stream) const
{
    identifier_->Print(stream);
    stream << " = ";
    value_->Print(stream);
}
