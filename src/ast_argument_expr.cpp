#include "ast_argument_expr.hpp"

void ArgumentExpression::EmitRISC(std::ostream &stream, Context &context) const
{
    context.callArgs.emplace_back(arg_);
}

void ArgumentExpression::Print(std::ostream &stream) const
{
    arg_->Print(stream);
};
