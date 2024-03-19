#include <cassert>
#include "ast_context.hpp"
#include "ast_node.hpp"

void Context::EnterScope()
{
    scopes.emplace_back();
}

void Context::ExitScope()
{
    scopes.pop_back();
}

Function *Context::DeclareFunction(
    std::string const &type,
    std::string const &name)
{
    currentFunction = functions.size();
    Function &fn = functions.emplace_back();
    fn.type = type;
    fn.name = name;
    return &fn;
}

size_t Context::SizeOfType(std::string const& /*type*/) const
{
    // Hardcoded to int only for now
    return 4;
}

Variable *Context::DeclareVariable(
    std::string const& type, std::string const& name)
{
    // Get a reference to the innermost scope
    // and create a new variable in it
    auto &inner = scopes.back();
    Variable &variable = inner.locals.emplace_back();
    variable.type = type;
    variable.name = name;
    assert(currentFunction != -1);

    size_t variableSize = SizeOfType(type);

    // Get a reference to the current function
    // so we can use it to find where to place the
    // new variable in the function's stack frame
    Function &fn = functions[currentFunction];

    // Make sure the offset is rounded to an
    // aligned boundary later when it handles different types
    fn.locals_size += variableSize;
    variable.offset = -fn.locals_size & -variableSize;

    return &variable;
}

Variable *Context::FindVariable(std::string const &name)
{
    for (size_t i = scopes.size(); i > 0; --i)
    {
        auto &scope = scopes[i-1];
        for (Variable& var : scope.locals)
        {
            if (var.name == name)
                return &var;
        }
    }

    return nullptr;
}
