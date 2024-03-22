#include <sstream>
#include "ast_function_definition.hpp"

void FunctionDefinition::EmitRISC(std::ostream &stream, Context &context) const
{
    // Emit assembler directives.

    std::stringstream ss;
    declarator_->Print(ss);
    std::string name = ss.str();
    ss.str("");

    declaration_specifiers_->Print(ss);
    std::string type = ss.str();
    ss.str("");

    // TODO: these are just examples ones, make sure you understand
    // the concept of directives and correct them.
    stream << ".text" << std::endl;
    stream << ".globl " << name << std::endl;

    Function* fn = context.DeclareFunction(type, name);

    context.EnterScope();

    declarator_->EmitRISC(stream, context);

    stream << "# Allocate space to save ra and caller's s0\n";
    stream << "addi sp,sp,-8\n";
    stream << "# Save caller's ra\n";
    stream << "sw ra,4(sp)\n";
    stream << "# Save caller's s0\n";
    stream << "sw s0,0(sp)\n";

    stream << "# Copy stack pointer into frame pointer\n";
    stream << "mv s0,sp\n";

    // Create the parameter variables
    // Node *paramList = context.functionParameters;
    // if (paramList)
    //     paramList->EmitRISC(stream, context);

    int parameter_nr = 0;

    std::vector<Variable*> parameterInstances;

    // Expand the vector capacity so the instances won't move
    context.ReserveVariables(context.parameterDecls.size());

    // Declare a variable for each parameter
    for (auto &var : context.parameterDecls)
    {
        Variable *parameterVariable = context.DeclareVariable(
            var.type, var.name);

        parameterInstances.emplace_back(parameterVariable);
    }

    if (!parameterInstances.empty()) {
        // Variable *firstParameter = parameterInstances.front();
        Variable *lastParameter = parameterInstances.back();
        // size_t end = lastParameter->offset +
        //     context.SizeOfType(lastParameter->type);
        // size_t start = firstParameter->offset;
        // ssize_t alloc = end - start;
        ssize_t alloc = lastParameter->offset;
        stream << "# Allocate space for the parameters\n";
        stream << "addi sp,sp," << alloc << "\n";
    }

    for (auto &var : context.parameterDecls)
    {
        Variable *parameterVariable =
            parameterInstances[parameter_nr];
        // Generate code to copy the parameter register
        // into the variable (in the stack frame)
        stream << "# Store parameter " << parameter_nr <<
            " ( " << parameterVariable->name << ")\n";
        stream << "sw a" << parameter_nr++ << "," <<
            parameterVariable->offset << "(s0)\n";
    }

    if (compound_statement_ != nullptr)
    {
        compound_statement_->EmitRISC(stream, context);
    }

    context.ExitScope();
    context.currentFunction = -1;
    context.parameterDecls.clear();

    // Just in case they don't have any `return` statement

    stream << "mv sp,s0\n";
    stream << "lw s0,(sp)\n";
    stream << "lw ra,4(sp)\n";
    stream << "addi sp,sp,8\n";

    stream << "ret" << std::endl;
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
