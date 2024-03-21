#include "ast_switch_statement.hpp"

void SwitchStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    expression_->EmitRISC(stream, context);

    stream << "mv t0, a0" << std::endl;

    statement_->EmitRISC(stream, context);

}

void SwitchStatement::Print(std::ostream &stream) const
{
    stream << "switch(";
    expression_->Print(stream);
    stream << "){" << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
}

void CaseStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    constant_expression_->EmitRISC(stream, context);

    std::string NotEqual = context.NewLabel();

    stream << "BNE t0, a0, " << NotEqual << std::endl;

    statement_->EmitRISC(stream, context);

    stream << NotEqual << ":" << std::endl;

}

void CaseStatement::Print(std::ostream &stream) const
{
    stream << "case ";
    constant_expression_->Print(stream);
    stream << ":" << std::endl;
    statement_->Print(stream);
}

void DefaultStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    statement_->EmitRISC(stream, context);

}

void DefaultStatement::Print(std::ostream &stream) const
{
    stream << "default: ";
    statement_->Print(stream);
}
