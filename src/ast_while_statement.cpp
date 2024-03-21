#include "ast_while_statement.hpp"

void WhileStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    std::string StartLabel = context.NewLabel();
    std::string EndLabel = context.NewLabel();


    stream << StartLabel << ":" << std::endl;

    expression_->EmitRISC(stream, context);

    stream << "beqz a0, " << EndLabel << std::endl;

    statement_->EmitRISC(stream, context);

    stream << "beqz zero, " << StartLabel << std::endl;

    stream << EndLabel << ":" << std::endl;
}

void WhileStatement::Print(std::ostream &stream) const
{
    stream << "while(";
    expression_->Print(stream);
    stream << "){" << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
}

void DoWhileStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    std::string StartLabel = context.NewLabel();
    std::string EndLabel = context.NewLabel();

    stream << StartLabel << ":" << std::endl;

    statement_->EmitRISC(stream, context);

    expression_->EmitRISC(stream, context);

    stream << "beqz a0, " << EndLabel << std::endl;

    stream << "beqz zero, " << StartLabel << std::endl;

    stream << EndLabel << ":" << std::endl;
}

void DoWhileStatement::Print(std::ostream &stream) const
{
    stream << "do{";
    statement_->Print(stream);
    stream << "}while(";
    expression_->Print(stream);
    stream << ");" << std::endl;
}
