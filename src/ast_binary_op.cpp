#include <sstream>
#include <cassert>

#include "ast_binary_op.hpp"
#include <unistd.h>


void BinaryOp::EmitRISC(std::ostream &stream, Context &context) const
{
    char const *insn = nullptr;
    switch (oper_) {
        case OP_ADD: insn = "add"; break;
        case OP_SUB: insn = "sub"; break;
        case OP_MUL: insn = "mul"; break;
        case OP_DIV: insn = "div"; break;
        case OP_BITAND: insn = "and"; break;
        case OP_BITOR: insn = "orb"; break;
        case OP_BITXOR: insn = "xor"; break;
        case OP_LEFT: insn = "shl"; break;
        case OP_RIGHT: insn = "sra"; break;
        default: assert(!"FIXME: Unhandled operator");
    }

    stream << "# Evaluating left side for " << insn << "\n";
    lhs_->EmitRISC(stream, context);
    stream << "# Pushing left side result for " << insn << "\n";
    stream << "addi sp,sp,-4\n";
    stream << "sw a0,0(sp)\n";
    stream << "# Eva;iatomg side result for " << insn << "\n";
    rhs_->EmitRISC(stream, context);
    stream << "# Popping saved left side result for " << insn << "\n";
    stream << "lw a1,0(sp)\n";
    stream << "addi sp,sp,4\n";

    stream << "# Executing operation for " << insn << "\n";
    stream << insn << " a0,a1,a0\n";
}

void BinaryOp::Print(std::ostream &stream) const
{
    lhs_->Print(stream);
    char const *optxt = nullptr;
    switch (oper_) {
        case OP_ADD: optxt = "+"; break;
        case OP_SUB: optxt = "-"; break;
        case OP_MUL: optxt = "*"; break;
        case OP_DIV: optxt = "/"; break;
        case OP_BITAND: optxt = "&"; break;
        case OP_BITOR: optxt = "|"; break;
        case OP_BITXOR: optxt = "^"; break;
        case OP_LEFT: optxt = "<<"; break;
        case OP_RIGHT: optxt = ">>"; break;
        default: assert(!"FIXME: Unhandled operator");
    }
    stream << optxt << "\n";
    rhs_->Print(stream);
}
