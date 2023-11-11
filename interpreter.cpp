#include "interpreter.hpp"

// system includes
#include <stack>
#include <stdexcept>
#include <iostream>
#include <algorithm>

// module includes
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"


//class constructor
Interpreter::Interpreter() {}

bool Interpreter::parse(std::istream & expression) noexcept
{
    //check if the first character is open paranthesis '('
    char firstChar = expression.peek();
    if (firstChar != '(' && firstChar != ';')
    {
        return false;
    }

    TokenSequenceType tokens = tokenize(expression);
    auto iter = tokens.begin();
    if (iter == tokens.end())
    {
        return false; // Empty input
    }

    try
    {
        ast = parseExpression(iter, tokens.end());

        // After successfully parsing an expression, there should be no tokens left.
        if (iter != tokens.end())
        {
            return false; // Extra tokens found
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

Expression Interpreter::eval()
{
    if (ast.head.type == NoneType)
    {
        throw InterpreterSemanticError("Error: No AST to evaluate.");
    }

    return evaluateExpression(ast);
}

/*
 * Parses and constructs an Expression from a sequence of tokens.
 *
 * This function is responsible for recursively parsing tokens to build a structured
 * Expression object. It handles different types of tokens, including atomic values,
 * parentheses, and operations. If encountered, it validates the token sequence and
 * constructs an Expression accordingly.
 */
Expression Interpreter::parseExpression(TokenIteratorType& token, TokenIteratorType end)
{
    if (token == end)
    {
        throw InterpreterSemanticError("Error: unexpected end of input.");
    }
    std::string currentToken = *token; // Safe dereferencing

    if (currentToken == "(")
    {
        ++token;
        if (token == end || *token == ")")
        {
            throw InterpreterSemanticError("Error: empty expression.");
        }
        currentToken = *token;

        Atom potentialAtom;

        if (!token_to_atom(currentToken, potentialAtom))
        {
            throw InterpreterSemanticError("Error: Invalid token");
        }

        if (token_to_atom(currentToken, potentialAtom))
        {
            // If it's an atomic expression like True, False, or a number, return it
            if (potentialAtom.type == BooleanType || potentialAtom.type == NumberType)
            {
                ++token;
                if (token == end || *token != ")")
                {
                    throw InterpreterSemanticError("Error: expected closing parenthesis after atomic expression.");
                }
                ++token;
                return Expression(potentialAtom);
            }
        }
        ++token;

        //Continue parsing the operands for this operation
        std::vector<Expression> operands;
        while (token != end && *token != ")")
        {
            operands.push_back(parseExpression(token, end));
        }

        if (token == end)
        {
            throw InterpreterSemanticError("Error: expected closing parenthesis.");
        }

        ++token;
        return Expression(currentToken, operands);
    }

    if (currentToken != ")")
    {
        Atom atom;
        if (!token_to_atom(currentToken, atom))
        {
            throw InterpreterSemanticError("Error: invalid token.");
        }
        ++token;
        return Expression(atom);
    }

    throw InterpreterSemanticError("Error: Failed to parse.");
}

/**
 * Parses and constructs an Expression from a sequence of tokens.
 *
 * This function is responsible for recursively parsing tokens to build a structured
 * Expression object. It handles different types of tokens, including atomic values,
 * parentheses, and operations. If encountered, it validates the token sequence and
 * constructs an Expression accordingly.
 */
Expression Interpreter::evaluateExpression(const Expression& expr)
{
    // If the expression is atomic (has no tail):
    if (expr.tail.empty())
    {
        // If the head is a symbol:
        if (expr.head.type == SymbolType)
        {
            return env.get(expr.head.value.sym_value);
        }
        return expr;  // If the head isn't a symbol
    }
    // If the expression is not atomic (has a tail):
    if (!expr.tail.empty())
    {
        // The head should be an operation or procedure.
        if (expr.head.type == SymbolType)
        {
            std::string symbol = expr.head.value.sym_value;

            // Special handling for special forms
            if (symbol == "if")
            {
                if (expr.tail.size() != 3)
                {
                    throw InterpreterSemanticError("Error: Incorrect number of arguments for 'if'.");
                }
                Expression condition = evaluateExpression(expr.tail[0]);
                if (condition.head.type != BooleanType)
                {
                    throw InterpreterSemanticError("Error: Conditional in 'if' is not a boolean.");
                }
                if (condition.head.value.bool_value)
                {
                    return evaluateExpression(expr.tail[1]);
                }
                if (!condition.head.value.bool_value)
                {
                    return evaluateExpression(expr.tail[2]);
                }
            }
            else if (symbol == "begin")
            {
                if (expr.tail.empty())
                {
                    throw InterpreterSemanticError("Error: 'begin' requires at least one argument.");
                }
                Expression lastExpr;
                for (const auto& e : expr.tail)
                {
                    lastExpr = evaluateExpression(e);
                }
                return lastExpr;
            }
            else if (symbol == "define")
            {
                if (expr.tail.size() != 2 || expr.tail[0].head.type != SymbolType)
                {
                    throw InterpreterSemanticError("Error: Incorrect use of 'define'.");
                }

                // Extract the symbol string from the first item in the tail.
                std::string variable = expr.tail[0].head.value.sym_value;

                if (isSymbolStringDefined(variable))
                {
                    throw InterpreterSemanticError("Error: Variable already exists");
                }

                std::string symbol_to_define = expr.tail[0].head.value.sym_value;
                std::vector<std::string> specialForms = { "define", "if", "begin" };
                std::vector<std::string> builtInSymbols = { "pi", "+", "-", "*", "/" };

                if ((std::find(specialForms.begin(), specialForms.end(), symbol_to_define) != specialForms.end()) || (std::find(builtInSymbols.begin(), builtInSymbols.end(), symbol_to_define) != builtInSymbols.end()))
                {
                    throw InterpreterSemanticError("Error: Cannot redefine special form or built-in symbol.");
                }

                Expression value = evaluateExpression(expr.tail[1]);
                env.addSymbol(symbol_to_define, value);
                return value;
            }
            else
            {
                // For other symbols, evaluate as procedures
                std::vector<Expression> args;
                for (const auto& e : expr.tail)
                {
                    args.push_back(evaluateExpression(e));
                }
                return env.evaluateProcedure(symbol, args);
            }
        }
        else
        {
            throw InterpreterSemanticError("Error: Head of expression is not a symbol.");
        }
    }
}

// Reset environment to its default state
void Interpreter::resetEnvironment()
{
    env = Environment();
}

//Checks if a variable already exists in our environment
//returns true if variable exists and false otherwise. 
bool Interpreter::isSymbolStringDefined(std::string variable)
{
    // Check if the symbol string is in the envmap.
    try
    {
        Expression test = env.get(variable);
    }
    catch (...)
    {
        return false;
    }

    return true;
}