#include "ast_for_statement.hpp"

void ForStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    expression_statement_initialize_->EmitRISC(stream, context);

    std::string startlabel = context.NewLabel();
    std::string endlabel = context.NewLabel();

    stream << startlabel << ":" << std::endl;

    expression_statement_loop_->EmitRISC(stream, context);

    stream << "beqz a0, " << endlabel << std::endl;

    statement_->EmitRISC(stream, context);

    stream << "beqz zero, " << startlabel << std::endl;

    stream << endlabel << ":" << std::endl;
}

void ForStatement::Print(std::ostream &stream) const
{
    stream << "for(";
    expression_statement_initialize_->Print(stream);
    stream << "; ";
    expression_statement_loop_->Print(stream);
    stream << "){" << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
}

void ExprForStatement::EmitRISC(std::ostream &stream, Context &context) const
{

    expression_statement_initialize_->EmitRISC(stream, context);

    std::string startname = context.NewLabel();
    std::string endname = context.NewLabel();

    stream << startname << ":" << std::endl;

    expression_statement_loop_->EmitRISC(stream, context);

    stream << "beqz a0, " << endname << std::endl;

    statement_->EmitRISC(stream, context);

    expression_->EmitRISC(stream, context);

    stream << "beqz zero, " << startname << std::endl;

    stream << endname << ":" << std::endl;
}

void ExprForStatement::Print(std::ostream &stream) const
{
    stream << "for(";
    expression_statement_initialize_->Print(stream);
    stream << "; ";
    expression_statement_loop_->Print(stream);
    stream << "; ";
    expression_->Print(stream);
    stream << "){" << std::endl;
    statement_->Print(stream);
    stream << "}" << std::endl;
}
