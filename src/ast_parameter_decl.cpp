#include <sstream>
#include "ast_parameter_decl.hpp"

void ParameterDecl::EmitRISC(std::ostream &stream, Context &context) const
{
    std::ostringstream ss;

    declaration_specifiers_->Print(ss);
    std::string type = ss.str();
    ss.str("");

    declarator_->EmitRISC(ss, context);
    std::string name = ss.str();
    ss.str("");

    Variable &parameterVariable =
        context.parameterDecls.emplace_back();
    parameterVariable.type = std::move(type);
    parameterVariable.name = std::move(name);
    parameterVariable.offset = 0;
    parameterVariable.inMemory = true;
}

void ParameterDecl::Print(std::ostream &stream) const
{
    declaration_specifiers_->Print(stream);
    stream << ' ';
    declarator_->Print(stream);
}
