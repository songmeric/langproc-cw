#include <sstream>
#include "ast_variable_ref.hpp"

void VariableReference::EmitRISC(std::ostream &stream, Context &context) const
{
    std::ostringstream ss;
    variable_identifier_->EmitRISC(ss, context);
    std::string name = ss.str();
    Variable *v = context.FindVariable(name);

    if (!v) {
        // // Maybe it's a function
        // Function *fn = context.FindFunction(name);
        // if (fn) {
        //     stream << "la a0," << fn->name << "\n";
        //     return;
        // }
        stream << "la a0," << name << "\n";
        return;
    }

    if (!context.variableStore) {
        stream << "# Load from variable \"" << v->name << "\"\n";
        stream << "lw a0," << v->offset << "(s0)\n";
    } else {
        stream << "# Store to variable \"" << v->name << "\"\n";
        stream << "sw a0," << v->offset << "(s0)\n";
        context.variableStore = false;
    }
}

void VariableReference::Print(std::ostream &stream) const
{
    variable_identifier_->Print(stream);
}
