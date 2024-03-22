#include <sstream>
#include <cassert>

#include "ast_binary_op.hpp"
#include <unistd.h>

#include <cstring>


void BinaryOp::EmitRISC(std::ostream &stream, Context &context) const
{
    // Handle assignments in a way that reuses the
    // code for non-assignment
    bool assignment = IsAssignment();

    Operator op = oper_;

    if (assignment && oper_ != OP_ASSIGN) {
        // The enum is set up so the X enum is equal
        // to the integer value of X minus one
        op = Operator((int)op - 1);
    }

    char const *insn = nullptr;
    switch (op) {
        case OP_ASSIGN: insn = "sw"; break;
        case OP_ADD: insn = "add"; break;
        case OP_SUB: insn = "sub"; break;
        case OP_MUL: insn = "mul"; break;
        case OP_DIV: insn = "div"; break;
        case OP_BITAND: insn = "and"; break;
        case OP_BITOR: insn = "or"; break;  // changed from orb to or
        case OP_BITXOR: insn = "xor"; break;
        case OP_LEFT: insn = "shl"; break;
        case OP_RIGHT: insn = "sra"; break;

        case OP_CMPEQ: insn = "beq"; break;     ////
        case OP_CMPNE: insn = "bne"; break;
        case OP_CMPLT: insn = "blt"; break;
        case OP_CMPGT: insn = "bgt"; break;
        case OP_CMPGE: insn = "bge"; break;
        case OP_CMPLE: insn = "ble"; break;
        case OP_LOGOR: insn = "logor"; break;
        case OP_LOGAND: insn = "logand"; break; ////

        default: assert(!"FIXME: Unhandled operator");
    }

    // left hand side is expected to be in a1
    // right hand side is expected to be in a0

    if (assignment) {
        if (op == OP_ASSIGN) {
            // Evaluate value to be assigned
            stream << "# Evaluating right side result for " << insn << "\n";
            rhs_->EmitRISC(stream, context);

            // Tell the VariableReference that it is a store
            context.variableStore = true;
            lhs_->EmitRISC(stream, context);
            return;
        }

        // Load the variable as left hand side
        lhs_->EmitRISC(stream, context);
    } else {
        stream << "# Evaluating left side for " << insn << "\n";
        lhs_->EmitRISC(stream, context);
    }

    stream << "# Pushing left side result for " << insn << "\n";
    stream << "addi sp,sp,-4\n";
    stream << "sw a0,0(sp)\n";
    stream << "# Evaluating right side result for " << insn << "\n";
    rhs_->EmitRISC(stream, context);
    stream << "# Popping saved left side result for " << insn << "\n";
    stream << "lw a1,0(sp)\n";
    stream << "addi sp,sp,4\n";

    stream << "# Executing operation for " << insn << "\n";

    if ((strcmp(insn, "beq") == 0)||(strcmp(insn, "bne") == 0)||        ////
        (strcmp(insn, "blt") == 0)||(strcmp(insn, "bgt") == 0)||
        (strcmp(insn, "bge") == 0)||(strcmp(insn, "ble") == 0)){
        std::string True = context.NewLabel();
        std::string False = context.NewLabel();

        stream << insn << " a1,a0," << True << "\n";
        stream << "li a0,0\n";
        stream << "j " << False << "\n";
        stream << True <<":\n";
        stream << "li a0,1\n";
        stream << False <<":\n";
    }
    else if(strcmp(insn, "logor") == 0){
        std::string True = context.NewLabel();
        std::string False = context.NewLabel();

        stream << "or a0,a1,a0\n";
        stream << "bnez a0," << True << "\n";
        stream << "li a0,0\n";
        stream << "j " << False << "\n";
        stream << True << ":\n";
        stream << "li a0,1\n";
        stream << False << ":\n";
    }
    else if(strcmp(insn, "logand") == 0){
        std::string True = context.NewLabel();
        std::string False = context.NewLabel();

        stream << "and a0,a1,a0\n";
        stream << "bnez a0," << True << "\n";
        stream << "li a0,0\n";
        stream << "j " << False << "\n";
        stream << True << ":\n";
        stream << "li a0,1\n";
        stream << False << ":\n";
    }
    else{   ////
        stream << insn << " a0,a1,a0\n";
    }

    if (assignment) {
        context.variableStore = true;
        lhs_->EmitRISC(stream, context);
    }
}

void BinaryOp::Print(std::ostream &stream) const
{
    bool assignment = IsAssignment();

    Operator op = oper_;

    if (assignment && op != OP_ASSIGN) {
        op = Operator((int)op - 1);
    }

    lhs_->Print(stream);
    char const *optxt = nullptr;
    switch (op) {
        case OP_ASSIGN: optxt = ""; break;
        case OP_ADD: optxt = "+"; break;
        case OP_SUB: optxt = "-"; break;
        case OP_MUL: optxt = "*"; break;
        case OP_DIV: optxt = "/"; break;
        case OP_BITAND: optxt = "&"; break;
        case OP_BITOR: optxt = "|"; break;
        case OP_BITXOR: optxt = "^"; break;
        case OP_LEFT: optxt = "<<"; break;
        case OP_RIGHT: optxt = ">>"; break;

        case OP_CMPEQ: optxt = "=="; break;  ////
        case OP_CMPNE: optxt = "!="; break;
        case OP_CMPLT: optxt = "<"; break;
        case OP_CMPGT: optxt = ">"; break;
        case OP_CMPGE: optxt = ">="; break;
        case OP_CMPLE: optxt = "<="; break;
        case OP_LOGOR: optxt = "||"; break;
        case OP_LOGAND: optxt = "&&"; break; ////

        default: assert(!"FIXME: Unhandled operator");
    }
    stream << optxt << (assignment ? "=" : "");
    rhs_->Print(stream);
}

bool BinaryOp::IsAssignment() const
{
    switch (oper_) {
    case OP_ASSIGN:
    case OP_MULASSIGN:
    case OP_DIVASSIGN:
    case OP_MODASSIGN:
    case OP_ADDASSIGN:
    case OP_SUBASSIGN:
    case OP_LEFTASSIGN:
    case OP_RIGHTASSIGN:
    case OP_ANDASSIGN:
    case OP_XORASSIGN:
    case OP_ORASSIGN:
        return true;
    }
    return false;
}
