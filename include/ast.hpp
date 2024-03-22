#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ast_direct_declarator.hpp"
#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include "ast_jump_statement.hpp"
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"
#include "ast_constant.hpp"
#include "ast_context.hpp"
#include "ast_init_declarator.hpp"
#include "ast_declaration_list.hpp"
#include "ast_compound_stmt.hpp"
#include "ast_variable_ref.hpp"

#include "ast_if_statement.hpp"
#include "ast_switch_statement.hpp"
#include "ast_while_statement.hpp"
#include "ast_for_statement.hpp"
#include "ast_binary_op.hpp"
#include "ast_parameter_decl.hpp"
#include "ast_function_expr.hpp"

extern Node *ParseAST(std::string file_name);

#endif
