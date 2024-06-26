// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

%code requires{
    #include "ast.hpp"
	#include <assert.h>

    extern Node *g_root;
    extern FILE *yyin;
    int yylex(void);
    void yyerror(const char *);
	int yylex_destroy(void);
}

// Represents the value associated with any kind of AST node.
%union{
  Node         *node;
  NodeList     *nodes;
  int          number_int;
  double       number_float;
  std::string  *string;
  yytokentype  token;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <node> function_definition primary_expression postfix_expression
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression expression constant_expression declaration declaration_specifiers
%type <node> init_declarator type_specifier struct_specifier struct_declaration argument_expression
%type <node> struct_declarator enum_specifier enumerator_list enumerator declarator direct_declarator pointer parameter_declaration
%type <node> identifier_list type_name abstract_declarator direct_abstract_declarator initializer initializer_list statement labeled_statement
%type <node> compound_statement expression_statement selection_statement iteration_statement jump_statement

%type <nodes> statement_list struct_declaration_list specifier_qualifier_list
%type <nodes> struct_declarator_list declaration_list init_declarator_list parameter_list
%type <nodes> argument_expression_list translation_unit external_declaration

%type <string> unary_operator assignment_operator storage_class_specifier

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER


%start ROOT
%%

ROOT
  : translation_unit { g_root = $1; }

translation_unit
	: external_declaration {
		$$ = new NodeList($1);
	}
	| translation_unit external_declaration {
		$1->PushBack($2);
		$$ = $1;
	}
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement {
		assert(!"Unimplemented!");
	}
	| declaration_specifiers declarator compound_statement {
		$$ = new FunctionDefinition($1, $2, $3);
	}
	| declarator declaration_list compound_statement {
		assert(!"Unimplemented!");
	}
	| declarator compound_statement {
		assert(!"Unimplemented!");
	}
	;


primary_expression
	: IDENTIFIER {
		$$ = new VariableReference(new Identifier(*$1));
		delete $1;
	}
	| INT_CONSTANT {
		$$ = new IntConstant($1);
	}
    | FLOAT_CONSTANT
	| STRING_LITERAL
	| '(' expression ')' { $$ = $2; }
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']' {
		$$ = new BinaryOp(nullptr, OP_DEREFERENCE,
			new BinaryOp($1, OP_ADD, $3));
	}
	| postfix_expression '(' ')' {
		$$ = new BinaryOp($1, OP_CALL, nullptr);
	}
	| postfix_expression '(' argument_expression_list ')' {
		$$ = new BinaryOp(new FunctionExpression($1), OP_CALL, $3);
	}
	| postfix_expression '.' IDENTIFIER {
		$$ = new BinaryOp($1, OP_MEMBER, new Identifier(*$3));
	}
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP // { $$ = new PostfixOp($1, OP_INC); }
	| postfix_expression DEC_OP // { $$ = new PostfixOp($1, OP_DEC); }
	;

argument_expression
	: expression
	;

//	: assignment_expression
argument_expression_list
	: argument_expression {
		$$ = new NodeList($1);
	}
	| argument_expression_list ',' argument_expression {
		$1->PushBack($3);
		$$ = $1;
	}
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression {
		$$ = new BinaryOp($1, OP_MUL, $3);
	}
	;
	| multiplicative_expression '/' cast_expression {
		$$ = new BinaryOp($1, OP_DIV, $3);
	}
	;
	| multiplicative_expression '%' cast_expression {
		$$ = new BinaryOp($1, OP_MOD, $3);
	}
	;
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression {
		$$ = new BinaryOp($1, OP_ADD, $3);
	}
	;
	| additive_expression '-' multiplicative_expression {
		$$ = new BinaryOp($1, OP_SUB, $3);
	}
	;
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression {
		$$ = new BinaryOp($1, OP_LEFT, $3);
	}
	;
	| shift_expression RIGHT_OP additive_expression {
		$$ = new BinaryOp($1, OP_RIGHT, $3);
	}
	;
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression {
		$$ = new BinaryOp($1, OP_CMPLT, $3);
	}
	;
	| relational_expression '>' shift_expression {
		$$ = new BinaryOp($1, OP_CMPGT, $3);
	}
	;
	| relational_expression LE_OP shift_expression {
		$$ = new BinaryOp($1, OP_CMPLE, $3);
	}
	;
	| relational_expression GE_OP shift_expression {
		$$ = new BinaryOp($1, OP_CMPGE, $3);
	}
	;
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression {
		$$ = new BinaryOp($1, OP_CMPEQ, $3);
	}
	;
	| equality_expression NE_OP relational_expression {
		$$ = new BinaryOp($1, OP_CMPNE, $3);
	}
	;
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression {
		$$ = new BinaryOp($1, OP_BITAND, $3);
	}
	;
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression {
		$$ = new BinaryOp($1, OP_BITXOR, $3);
	}
	;
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression {
		$$ = new BinaryOp($1, OP_BITOR, $3);
	}
	;
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression {
		$$ = new BinaryOp($1, OP_LOGAND, $3);
	}
	;
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression {
		$$ = new BinaryOp($1, OP_LOGOR, $3);
	}
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression '=' assignment_expression {
		$$ = new BinaryOp($1, OP_ASSIGN, $3);
	}
	| unary_expression MUL_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_MULASSIGN, $3);
	}
	| unary_expression DIV_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_DIVASSIGN, $3);
	}
	| unary_expression MOD_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_MODASSIGN, $3);
	}
	| unary_expression ADD_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_ADDASSIGN, $3);
	}
	| unary_expression SUB_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_SUBASSIGN, $3);
	}
	| unary_expression LEFT_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_LEFTASSIGN, $3);
	}
	| unary_expression RIGHT_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_RIGHTASSIGN, $3);
	}
	| unary_expression AND_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_ANDASSIGN, $3);
	}
	| unary_expression XOR_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_XORASSIGN, $3);
	}
	| unary_expression OR_ASSIGN assignment_expression {
		$$ = new BinaryOp($1, OP_ORASSIGN, $3);
	}
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression {
		$$ = new BinaryOp($1, OP_COMMA, $3);
	}
	;

constant_expression
	: conditional_expression
	;

declaration
	: declaration_specifiers ';' { $$ = $1; }
	| declaration_specifiers init_declarator_list ';' {
		$$ = new DeclarationList($1, $2);
	}

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier { $$ = $1; }
	| type_specifier declaration_specifiers
	;

init_declarator_list
	: init_declarator { $$ = new NodeList($1); }
	| init_declarator_list ',' init_declarator {
		$1->PushBack($3);
		$$ = $1;
	}
	;


init_declarator
	: declarator {
		$$ = new InitDeclarator($1, nullptr);
	}
	| declarator '=' initializer {
		$$ = new InitDeclarator($1, $3);
	}
	;
storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT {
		$$ = new TypeSpecifier("int");
	}
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
  | struct_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_specifier
	: STRUCT IDENTIFIER '{' struct_declaration_list '}'
	| STRUCT '{' struct_declaration_list '}'
	| STRUCT IDENTIFIER
	;

struct_declaration_list
	: struct_declaration {
		$$ = new NodeList($1);
	}
	| struct_declaration_list struct_declaration {
		$1->PushBack($2); $$ = $1;
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' {
		assert(!"Unimplemented");
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list {
		$2->PushBack($1); $$ = $2;
	}
	| type_specifier {
		$$ = new NodeList($1);
	}
	;

struct_declarator_list
	: struct_declarator { $$ = new NodeList($1); }
	| struct_declarator_list ',' struct_declarator { $1->PushBack($3); $$ = $1; }
	;

struct_declarator
	: declarator
	| ':' constant_expression {
		assert(!"Unimplemented");
	}
	| declarator ':' constant_expression {
		assert(!"Unimplemented");
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}' {
		assert(!"Unimplemented");
	}
	| ENUM IDENTIFIER '{' enumerator_list '}' {
		assert(!"Unimplemented");
	}
	| ENUM IDENTIFIER {
		assert(!"Unimplemented");
	}
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator {
		assert(!"Unimplemented");
	}
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression {
		assert(!"Unimplemented");
	}
	;

declarator
	: pointer direct_declarator {
		assert(!"Unimplemented");
	}
	| direct_declarator { $$ = $1; }
	;


direct_declarator
	: IDENTIFIER {
		$$ = new Identifier(*$1);
		delete $1;
	}
	| '(' declarator ')' { $$ = $2; }
	| direct_declarator '[' constant_expression ']' {
		assert(!"Unimplemented");
	}
	| direct_declarator '[' ']' {
		assert(!"Unimplemented");
	}
	| direct_declarator '(' parameter_list ')' {
		$$ = new DirectDeclarator($1, $3);
	}
	| direct_declarator '(' identifier_list ')' {
		assert(!"Unimplemented");
	}
	| direct_declarator '(' ')' {
		$$ = new DirectDeclarator($1, nullptr);
	}
	;

pointer
	: '*'
	| '*' pointer {
		assert(!"Unimplemented");
	}
	;

parameter_list
	: parameter_declaration {
		$$ = new NodeList($1);
	}
	| parameter_list ',' parameter_declaration {
		$1->PushBack($3);
		$$ = $1;
	}
	;

parameter_declaration
	: declaration_specifiers declarator {
		$$ = new ParameterDecl($1, $2);
	}
	| declaration_specifiers abstract_declarator {
		assert(!"Unimplemented");
	}
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER {
		$$ = new Identifier(*$1);
		delete $1;
	}
	| identifier_list ',' IDENTIFIER {
		assert(!"Unimplemented");
	}
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator {
		assert(!"Unimplemented");
	}
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator {
		assert(!"Unimplemented");
	}
	| pointer direct_abstract_declarator {
		assert(!"Unimplemented");
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' { $$ = $2; }
	| '[' ']' {
		assert(!"Unimplemented");
	}
	| '[' constant_expression ']' { $$ = $2; }
	| direct_abstract_declarator '[' ']' {
		assert(!"Unimplemented");
	}
	| direct_abstract_declarator '[' constant_expression ']' {
		assert(!"Unimplemented");
	}
	| '(' ')' {
		assert(!"Unimplemented");
	}
	| '(' parameter_list ')' { $$ = $2; }
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_list ')' {
		assert(!"Unimplemented");
	}
	;

initializer
	: assignment_expression
	| '{' initializer_list '}' {
		assert(!"Unimplemented");
	}
	| '{' initializer_list ',' '}' {
		assert(!"Unimplemented");
	}
	;

initializer_list
	: initializer
	| initializer_list ',' initializer {
		assert(!"Unimplemented");
	}
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement { $$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement {
		assert(!"Unimplemented");
	}
	| CASE constant_expression ':' statement {
		$$ = new CaseStatement($2, $4);
	}
	| DEFAULT ':' statement {
		$$ = new DefaultStatement($3);
	}
	;

compound_statement
	: '{' '}' {
		$$ = new CompoundStatement(nullptr, nullptr);
	}
	| '{' statement_list '}' {
		$$ = new CompoundStatement(nullptr, $2);
	}
	| '{' declaration_list '}' {
		$$ = new CompoundStatement($2, nullptr);
	}
	| '{' declaration_list statement_list '}'  {
		$$ = new CompoundStatement($2, $3);
	}
	;

declaration_list
	: declaration { $$ = new NodeList($1); }
	| declaration_list declaration { $1->PushBack($2); $$=$1; }
	;

statement_list
	: statement { $$ = new NodeList($1); }
	| statement_list statement { $1->PushBack($2); $$=$1; }
	;

expression_statement
	: ';'
	| expression ';' { $$ = $1; }
	;

selection_statement
	: IF '(' expression ')' statement {
		$$ = new IfStatement($3, $5);
	}
	| IF '(' expression ')' statement ELSE statement {
		$$ = new IfElseStatement($3, $5, $7);
	}
	| SWITCH '(' expression ')' statement {
		$$ = new SwitchStatement($3, $5);
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement {
		$$ = new WhileStatement($3, $5);
	}
	| DO statement WHILE '(' expression ')' ';' {
		$$ = new DoWhileStatement($2, $5);
	}
	| FOR '(' expression_statement expression_statement ')' statement {
		$$ = new ForStatement($3, $4, $6);
	}
	| FOR '(' expression_statement expression_statement expression ')' statement {
		$$ = new ExprForStatement($3, $4, $5, $7);
	}
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';' {
		$$ = new ReturnStatement(nullptr);
	}
	| RETURN expression ';' {
		$$ = new ReturnStatement($2);
	}
	;



%%

Node *g_root;

Node *ParseAST(std::string file_name)
{
	extern int yydebug;
	yydebug = 1;
  yyin = fopen(file_name.c_str(), "r");
  if(yyin == NULL){
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    exit(1);
  }
  g_root = nullptr;
  yyparse();
  fclose(yyin);
  yylex_destroy();
  return g_root;
}
