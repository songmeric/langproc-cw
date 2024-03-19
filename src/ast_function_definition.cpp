#include <sstream>
#include "ast_function_definition.hpp"

void FunctionDefinition::EmitRISC(std::ostream &stream, Context &context) const
{
    // Emit assembler directives.


    std::stringstream ss;
    declarator_->Print(ss);
    std::string name = ss.str();
    ss.clear();

    declaration_specifiers_->Print(ss);
    std::string type = ss.str();
    ss.clear();

    // TODO: these are just examples ones, make sure you understand
    // the concept of directives and correct them.
    stream << ".text" << std::endl;
    stream << ".globl " << name << std::endl;

    Function* fn = context.DeclareFunction(type, name);

    context.EnterScope();

    declarator_->EmitRISC(stream, context);

    stream << "# Allocate space to save s0\n";
    stream << "addi sp,sp,-4\n";

    stream << "# Save caller's s0\n";
    stream << "sw s0,0(sp)\n";

    stream << "# Copy stack pointer into frame pointer\n";
    stream << "mv s0,sp\n";

    if (compound_statement_ != nullptr)
    {
        compound_statement_->EmitRISC(stream, context);
    }

    context.ExitScope();
    context.currentFunction = -1;
}

void FunctionDefinition::Print(std::ostream &stream) const
{
    declaration_specifiers_->Print(stream);
    stream << " ";

    declarator_->Print(stream);
    stream << "() {" << std::endl;

    if (compound_statement_ != nullptr)
    {
        compound_statement_->Print(stream);
    }
    stream << "}" << std::endl;
}
