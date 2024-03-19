#ifndef AST_CONTEXT_HPP
#define AST_CONTEXT_HPP

#include <string>
#include <vector>
#include <map>

class Variable
{
public:
    std::string type;
    std::string name;
    size_t offset = 0;
    bool inMemory = true;
};

class Scope
{
public:
    std::vector<Variable> locals;
    size_t localsSize;
};

class Function {
public:
    std::string type;
    std::string name;
    std::vector<Variable> parameters;
    size_t locals_size = 0;
};

// An object of class Context is passed between AST nodes during compilation.
// This can be used to pass around information about what's currently being
// compiled (e.g. function scope and variable names).
class Context
{
    /* TODO decide what goes inside here */
public:
    std::vector<Scope> scopes;

    void EnterScope();
    void ExitScope();

    Function* DeclareFunction(
        std::string const &type,
        std::string const &name);

    size_t SizeOfType(std::string const &) const;

    std::vector<Function> functions;

    int currentFunction = -1;

    Variable *DeclareVariable(
        std::string const &type,
        std::string const &name);

    std::string declarationListType;

    Variable *FindVariable(std::string const& name);
};

#endif
