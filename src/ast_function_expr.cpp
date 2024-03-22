#include <sstream>
#include "ast_function_expr.hpp"
short foo(int s)
{

}

void FunctionExpression::EmitRISC(std::ostream &stream, Context &context) const
{
    // This is overly simple, it only supports
    // IDENTIFIER on left hand side of calls
    std::ostringstream ss;
    fn_->Print(ss);
    std::string name = ss.str();

    stream << "call " << name << "\n";
}

void FunctionExpression::Print(std::ostream &stream) const
{
    fn_->Print(stream);
}
