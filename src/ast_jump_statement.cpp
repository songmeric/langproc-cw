#include "ast_jump_statement.hpp"

void ReturnStatement::EmitRISC(std::ostream &stream, Context &context) const
{
    if (expression_ != nullptr)
    {
        expression_->EmitRISC(stream, context);
    }


    stream << "mv sp,s0\n";
    stream << "lw s0,(sp)\n";
    stream << "addi sp,sp,4\n";

    stream << "ret" << std::endl;
}

void ReturnStatement::Print(std::ostream &stream) const
{
    stream << "return";
    if (expression_ != nullptr)
    {
        stream << " ";
        expression_->Print(stream);
    }
    stream << ";" << std::endl;
}
