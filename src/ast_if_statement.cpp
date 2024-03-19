#include "ast_if_statement.hpp"

void IfStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    expression_->EmitRISC(stream, context);

    stream << "beqz a0, end" << std::endl;  // find register that expression_->EmitRISC(stream, context); is saved into

    statement_->EmitRISC(stream, context);

    stream << "end:" << std::endl;
}

void IfStatement::Print(std::ostream &stream) const
{
    stream << "if ( ";
    expression_->Print(stream);
    stream << " ) { " << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
}

void IfElseStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    expression_->EmitRISC(stream, context);

    stream << "beqz a0, else" << std::endl;

    statement_->EmitRISC(stream, context);

    stream << "beqz zero, end" << std::endl;
    stream << "else:" << std::endl;

    elsestatement_->EmitRISC(stream, context);

    stream << "end:" << std::endl;

}

void IfElseStatement::Print(std::ostream &stream) const
{
    stream << "if ( ";
    expression_->Print(stream);
    stream << " ) { " << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
    stream << "else {";
    elsestatement_->Print(stream);
    stream << "}" << std::endl;

}
