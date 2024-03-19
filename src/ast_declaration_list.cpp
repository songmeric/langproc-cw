#include <sstream>
#include "ast_declaration_list.hpp"
#include <unistd.h>

void DeclarationList::EmitRISC(std::ostream &stream, Context &context) const
{
    std::ostringstream ss;
    type_->EmitRISC(ss, context);
    std::string type = ss.str();
    size_t sz = context.SizeOfType(type);

    context.declarationListType = type;

    // Variable allocation codegen expects sizeof(T) in a1
    //stream << "li a1," << sz << '\n';

    // Allocate (and possibly initialize) each variable
    list_->EmitRISC(stream, context);
}

void DeclarationList::Print(std::ostream &stream) const
{
    type_->Print(stream);
    stream << ' ';
    list_->Print(stream);
    stream << ";\n";
}
