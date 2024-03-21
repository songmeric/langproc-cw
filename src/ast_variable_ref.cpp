#include <sstream>
#include "ast_variable_ref.hpp"

void VariableReference::EmitRISC(std::ostream &stream, Context &context) const
{
    std::ostringstream ss;
    variable_identifier_->EmitRISC(ss, context);
    std::string name = ss.str();
    Variable *v = context.FindVariable(name);
    stream << "# Load variable \"" << v->name << "\"\n";
    stream << "lw a0," << v->offset << "(s0)\n";
}

void VariableReference::Print(std::ostream &stream) const
{
    variable_identifier_->Print(stream);
}
