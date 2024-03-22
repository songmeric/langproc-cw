#include <sstream>
#include "ast_init_declarator.hpp"

void InitDeclarator::EmitRISC(std::ostream &stream, Context &context) const
{
    std::ostringstream ss;

    identifier_->Print(ss);
    std::string name = ss.str();
    Variable *v = context.DeclareVariable(context.declarationListType, name);

    if(value_){
        stream << "# Evaluating value for initialization of " << v->name << "\n";
        value_->EmitRISC(stream, context);
    }
    stream << "# Allocating space for " << v->name << "\n";
    stream << "addi sp,sp,-4\n";
    if(value_){
        stream << "# Initializing" << v->name << "\n";
        stream << "sw a0," << v->offset << "(s0)\n";
    }
}

void InitDeclarator::Print(std::ostream &stream) const
{
    identifier_->Print(stream);
    if(value_) {
        stream << " = ";
        value_->Print(stream);
    }
}
