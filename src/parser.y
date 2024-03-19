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

%type <node> translation_unit external_declaration function_definition primary_expression postfix_expression argument_expression_list
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression expression constant_expression declaration declaration_specifiers
%type <node> init_declarator type_specifier struct_specifier struct_declaration
%type <node> struct_declarator enum_specifier enumerator_list enumerator declarator direct_declarator pointer parameter_list parameter_declaration
%type <node> identifier_list type_name abstract_declarator direct_abstract_declarator initializer initializer_list statement labeled_statement
%type <node> compound_statement expression_statement selection_statement iteration_statement jump_statement

%type <nodes> statement_list struct_declaration_list specifier_qualifier_list struct_declarator_list  declaration_list init_declarator_list

%type <string> unary_operator assignment_operator storage_class_specifier

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER


%start ROOT
%%

ROOT
  : translation_unit { g_root = $1; }

translation_unit
	: external_declaration { $$ = $1; }
	| translation_unit external_declaration
	;

external_declaration
	: function_definition { $$ = $1; }
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
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
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
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
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
	| expression ',' assignment_expression
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
	: declarator
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
		assert(!"Unimplemented");
	}
	| direct_declarator '(' identifier_list ')' {
		assert(!"Unimplemented");
	}
	| direct_declarator '(' ')' {
		$$ = new DirectDeclarator($1);
	}
	;

pointer
	: '*'
	| '*' pointer {
		assert(!"Unimplemented");
	}
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration {
		assert(!"Unimplemented");
	}
	;

parameter_declaration
	: declaration_specifiers declarator {
		assert(!"Unimplemented");
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
		assert(!"Unimplemented");
	}
	| DEFAULT ':' statement {
		assert(!"Unimplemented");
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
	: IF '(' expression ')' '{' statement '}' {
		$$ = new IfStatement($3, $6);
	}
	| IF '(' expression ')' '{' statement '}' ELSE '{' statement '}' {
		$$ = new IfElseStatement($3, $6, $10);
	}
	| SWITCH '(' expression ')' statement {
		assert(!"Unimplemented");
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement {
		assert(!"Unimplemented");
	}
	| DO statement WHILE '(' expression ')' ';' {
		assert(!"Unimplemented");
	}
	| FOR '(' expression_statement expression_statement ')' statement {
		assert(!"Unimplemented");
	}
	| FOR '(' expression_statement expression_statement expression ')' statement {
		assert(!"Unimplemented");
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
