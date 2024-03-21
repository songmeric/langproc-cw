#include "ast_if_statement.hpp"

void IfStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    expression_->EmitRISC(stream, context);

    std::string labelName = context.NewLabel();

    // find register that expression_->EmitRISC(stream, context); is saved into
    stream << "beqz a0, " << labelName << std::endl;

    statement_->EmitRISC(stream, context);

    stream << labelName << ":" << std::endl;
}

void IfStatement::Print(std::ostream &stream) const
{
    stream << "if(";
    expression_->Print(stream);
    stream << "){" << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
}

void IfElseStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    std::string elseLabel = context.NewLabel();
    std::string endLabel = context.NewLabel();

    expression_->EmitRISC(stream, context);

    stream << "beqz a0, " << elseLabel << std::endl;

    statement_->EmitRISC(stream, context);

    stream << "beqz zero, " << endLabel << std::endl;
    stream << elseLabel << ":" << std::endl;

    elsestatement_->EmitRISC(stream, context);

    stream << endLabel << ":" << std::endl;

}

void IfElseStatement::Print(std::ostream &stream) const
{
    stream << "if(";
    expression_->Print(stream);
    stream << "){" << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
    stream << "else{";
    elsestatement_->Print(stream);
    stream << "}" << std::endl;

}
