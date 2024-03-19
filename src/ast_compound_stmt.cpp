#include <sstream>
#include "ast_compound_stmt.hpp"

void CompoundStatement::EmitRISC(std::ostream &stream, Context &context) const
{
    // Emit assembler directives.
    if (decls_)
        decls_->EmitRISC(stream, context);
    if (stmts_)
        stmts_->EmitRISC(stream, context);
}

void CompoundStatement::Print(std::ostream &stream) const
{
    if (decls_)
        decls_->Print(stream);
    if (stmts_)
        stmts_->Print(stream);
}
