#include "ast_direct_declarator.hpp"

void DirectDeclarator::EmitRISC(std::ostream &stream, Context &context) const
{
    identifier_->EmitRISC(stream, context);
    stream << ":" << std::endl;

    if (parameters_)
        parameters_->EmitRISC(stream, context);
    context.functionParameters = parameters_;
}

void DirectDeclarator::Print(std::ostream &stream) const
{
    identifier_->Print(stream);
    // if (parameters_)
    //     parameters_->Print(stream);
}
