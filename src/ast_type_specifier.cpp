#include "ast_type_specifier.hpp"

void TypeSpecifier::EmitRISC(std::ostream &stream, Context &context) const
{
    // Do nothing
}

void TypeSpecifier::Print(std::ostream &stream) const
{
    stream << type_;
}
