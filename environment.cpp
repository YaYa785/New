#include "environment.hpp"

#include <cassert>
#include <cmath>

#include "interpreter_semantic_error.hpp"

//Functon that handles a logical negation procedure
Expression notProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 1 || args[0].type != BooleanType)
    {
        throw InterpreterSemanticError("Error: Invalid argument for not");
    }
    return Expression(!args[0].value.bool_value);
}

//Functon that handles a logical AND procedure
Expression andProcedure(const std::vector<Atom>& args)
{
    if (args.size() < 2)
    {
        throw InterpreterSemanticError("Error: Too few arguments for AND");
    }

    for (const auto& arg : args)
    {
        if (arg.type != BooleanType)
        {
            throw InterpreterSemanticError("Error: Invalid argument for AND");
        }
    }
    for (const auto& arg : args)
    {
        if (!arg.value.bool_value)
        {
            return Expression(false);
        }
    }
    return Expression(true);
}

//Functon that handles a logical OR procedure
Expression orProcedure(const std::vector<Atom>& args)
{
    if (args.size() < 2)
    {
        throw InterpreterSemanticError("Error: Too few arguments for OR");
    }

    for (const auto& arg : args)
    {
        if (arg.type != BooleanType)
        {
            throw InterpreterSemanticError("Error: Invalid argument type for or");
        }
    }
    for (const auto& arg : args)
    {
        if (arg.value.bool_value)
        {
            return Expression(true);
        }
    }

    return Expression(false);
}

//Functon that handles an arithmetic add procedure
Expression ADDProcedure(const std::vector<Atom>& args)
{
    double sum = 0.0;
    for (const auto& arg : args)
    {
        if (arg.type != NumberType || args.size() < 2)
        {
            throw InterpreterSemanticError("Error: Invalid argument for addition");
        }
        sum += arg.value.num_value;
    }
    return Expression(sum);
}

//Functon that handles an arithmetic subtract procedure
Expression subtractProcedure(const std::vector<Atom>& args)
{
    //Unary minus sign
    if (args.size() == 1)
    {
        if (args[0].type != NumberType)
        {
            throw InterpreterSemanticError("Error: Invalid argument for unary subtraction");
        }
        return Expression(-args[0].value.num_value);
    }

    //Binary Subtraction 
    if (args.size() == 2)
    {
        if (args[0].type != NumberType || args[1].type != NumberType)
        {
            throw InterpreterSemanticError("Error: Invalid arguments for binary subtraction");
        }
        return Expression(args[0].value.num_value - args[1].value.num_value);
    }

    throw InterpreterSemanticError("Error: Invalid number of arguments for subtraction");
}

//Functon that handles an arithmetic multiply procedure
Expression multiplyProcedure(const std::vector<Atom>& args)
{
    if (args.size() < 2)
    {
        throw InterpreterSemanticError("Error: Invalid number of arguments for multiplication");
    }

    double product = 1.0;
    for (const auto& arg : args)
    {
        if (arg.type != NumberType)
        {
            throw InterpreterSemanticError("Error: Invalid argument for multiplication");
        }
        product *= arg.value.num_value;
    }
    return Expression(product);
}

//Functon that handles an arithmetic divide procedure
Expression divideProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType || args[1].value.num_value == 0)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for division");
    }
    return Expression(args[0].value.num_value / args[1].value.num_value);
}

//Functon that handles a less than comparison procedure
Expression lessThanProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for < operation");
    }
    return Expression(args[0].value.num_value < args[1].value.num_value);
}

//Functon that handles a less than or equal procedure
Expression lessThanOrEqualProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for <= operation");
    }
    return Expression(args[0].value.num_value <= args[1].value.num_value);
}

//Functon that handles a greater than comparison procedure
Expression greaterThanProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for > operation");
    }
    return Expression(args[0].value.num_value > args[1].value.num_value);
}

//Functon that handles a greater than or equal comparison procedure
Expression greaterThanOrEqualProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for >= operation");
    }
    return Expression(args[0].value.num_value >= args[1].value.num_value);
}

//Functon that handles an equal comparison procedure
Expression equalProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for = operation");
    }
    return Expression(args[0].value.num_value == args[1].value.num_value);
}

//Functon that handles an arithmetic logarithmic procedure
Expression log10Procedure(const std::vector<Atom>& args)
{
    if (args.size() != 1 || args[0].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for log10 operation");
    }
    if (args[0].value.num_value <= 0)
    {
        throw InterpreterSemanticError("Error: Non-positive argument for log10");
    }
    return Expression(std::log10(args[0].value.num_value));
}

//Functon that handles an arithmetic power procedure
Expression powProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for pow operation");
    }
    return Expression(std::pow(args[0].value.num_value, args[1].value.num_value));
}

// Procedure to create a point
Expression pointProcedure(const std::vector<Atom>& args) 
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType) 
    {
        throw InterpreterSemanticError("Error: Invalid number of arguments for point, expected 2.");
    }
    return Expression(std::make_tuple(args[0].value.num_value, args[1].value.num_value));
}
//Procedure to create a line
Expression lineProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 2 || args[0].type != PointType || args[1].type != PointType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for line, expected two points.");
    }

    // Extract the points from the arguments
    Point startPoint = args[0].value.point_value;
    Point endPoint = args[1].value.point_value;

    // Create a Line object using the two points
    return Expression(std::make_tuple(startPoint.x, startPoint.y), std::make_tuple(endPoint.x, endPoint.y));
}


//Procedure to create arc
Expression arcProcedure(const std::vector<Atom>& args)
{
    if (args.size() != 3 || args[0].type != PointType || args[1].type != PointType || args[2].type != NumberType)
    {
        throw InterpreterSemanticError("Error: Invalid arguments for arc, expected two points and an angle.");
    }

    // Extract the points and angle from the arguments
    Point centerPoint = args[0].value.point_value;
    Point startPoint = args[1].value.point_value;
    double angle = args[2].value.num_value;

    // Create an Arc object using the two points and the angle
    return Expression(std::make_tuple(centerPoint.x, centerPoint.y), std::make_tuple(startPoint.x, startPoint.y), angle);
}



// Procedure for sin function
Expression sinProcedure(const std::vector<Atom>& args) 
{
    if (args.size() != 1 || args[0].type != NumberType) 
    {
        throw InterpreterSemanticError("Error: Invalid number of arguments for sin, expected 1.");
    }
    return Expression(std::sin(args[0].value.num_value));
}

// Procedure for cos function
Expression cosProcedure(const std::vector<Atom>& args) 
{
    if (args.size() != 1 || args[0].type != NumberType) 
    {
        throw InterpreterSemanticError("Error: Invalid number of arguments for cos, expected 1.");
    }
    return Expression(std::cos(args[0].value.num_value));
}

// Procedure for arctan function
Expression arctanProcedure(const std::vector<Atom>& args) 
{
    if (args.size() != 2 || args[0].type != NumberType || args[1].type != NumberType) 
    {
        throw InterpreterSemanticError("Error: Invalid number of arguments for arctan, expected 2.");
    }
    return Expression(std::atan2(args[0].value.num_value, args[1].value.num_value));
}

Expression drawProcedure(const std::vector<Atom>& args)
{
    if (args.empty())
    {
        throw InterpreterSemanticError("Error: Draw procedure expects at least one argument.");
    }

    // Check each argument to ensure it's a graphical object and "draw" them
    for (size_t i = 0; i < args.size(); ++i)
    {
        const auto& arg = args[i];

        if (arg.type == SymbolType)
        {
            if (arg.value.sym_value == "point" && (i + 2) < args.size() && args[i + 1].type == NumberType && args[i + 2].type == NumberType)
            {
                pointProcedure({ args[i + 1], args[i + 2] });
                i += 2; // skip the next two arguments
            }
            else if (arg.value.sym_value == "line" && (i + 2) < args.size() && args[i + 1].type == PointType && args[i + 2].type == PointType)
            {
                // Draw the points of the line
                pointProcedure({ args[i + 1] });
                pointProcedure({ args[i + 2] });
                lineProcedure({ args[i + 1], args[i + 2] });
                i += 2; // skip the next two arguments
            }
            else if (arg.value.sym_value == "arc" && (i + 3) < args.size() && args[i + 1].type == PointType && args[i + 2].type == PointType && args[i + 3].type == NumberType)
            {
                // Draw the points of the arc
                pointProcedure({ args[i + 1] });
                pointProcedure({ args[i + 2] });
                arcProcedure({ args[i + 1], args[i + 2], args[i + 3] });
                i += 3; // skip the next three arguments
            }
        }
        else if (arg.type == PointType)
        {
            return Expression(std::make_tuple(arg.value.point_value.x, arg.value.point_value.y));
        }
        else if (arg.type == LineType)
        {
            Point startPoint = arg.value.line_value.first;
            Point endPoint = arg.value.line_value.second;
            return Expression(std::make_tuple(startPoint.x, startPoint.y), std::make_tuple(endPoint.x, endPoint.y));
        }
        else if (arg.type == ArcType)
        {
            Point centerPoint = arg.value.arc_value.center;
            Point startPoint = arg.value.arc_value.start;
            double angle = arg.value.arc_value.span;
            return Expression(std::make_tuple(centerPoint.x, centerPoint.y), std::make_tuple(startPoint.x, startPoint.y), angle);
        }
        else
        {
            throw InterpreterSemanticError("Error: Invalid argument for draw procedure. Expected point, line, or arc.");
        }
    }

    // Return an expression of type None after "drawing" all objects
    return Expression();
}



//Class constructor
//Contains built in symbols and procedures
Environment::Environment()
{
    //Built in symbols
    addSymbol("pi", Expression(atan2(0, -1)));

    //Built in procedures
    addProcedure("not", notProcedure);
    addProcedure("and", andProcedure);
    addProcedure("or", orProcedure);
    addProcedure("<", lessThanProcedure);
    addProcedure("<=", lessThanOrEqualProcedure);
    addProcedure(">", greaterThanProcedure);
    addProcedure(">=", greaterThanOrEqualProcedure);
    addProcedure("=", equalProcedure);
    addProcedure("+", ADDProcedure);
    addProcedure("-", subtractProcedure);
    addProcedure("*", multiplyProcedure);
    addProcedure("/", divideProcedure);
    addProcedure("log10", log10Procedure);
    addProcedure("pow", powProcedure);

    // New procedures for graphical operations
    addProcedure("draw", drawProcedure);
    addProcedure("point", pointProcedure);
    addProcedure("line", lineProcedure);
    addProcedure("arc", arcProcedure);
    addProcedure("sin", sinProcedure);
    addProcedure("cos", cosProcedure);
    addProcedure("arctan", arctanProcedure);

}

//Adds a given symbol to the environment
void Environment::addSymbol(const Symbol& symbol, const Expression& value)
{
    EnvResult result;
    result.type = ExpressionType;
    result.exp = value;
    envmap[symbol] = result;
}

//Adds a given procedure to the environment
void Environment::addProcedure(const Symbol& symbol, Procedure procedure)
{
    EnvResult result;
    result.type = ProcedureType;
    result.proc = procedure;
    envmap[symbol] = result;
}

//Gets the procedure / symbol based on the given symbol
Expression Environment::get(const Symbol& symbol)
{
    auto it = envmap.find(symbol);
    if (it != envmap.end() && it->second.type == ExpressionType)
    {
        return it->second.exp;
    }
    throw InterpreterSemanticError("Error: Symbol not found or not associated with an expression.");
}

//Checks if the symbol is defined in the environment
bool Environment::isSymbolDefined(const Symbol& symbol)
{
    return envmap.find(symbol) != envmap.end();
}



//Evaluates procedure based on type
/*
* This function takes a symbol and a vector of arguments and looks up the symbol
* in the environment. If the symbol is found and associated with a procedure, it
* evaluates the procedure with the provided arguments. The procedure is expected
* to be a callable function that can accept a vector of Atom arguments and return
* an Expression result.
*/
Expression Environment::evaluateProcedure(const Symbol& symbol, const std::vector<Expression>& args)
{
    auto it = envmap.find(symbol);
    if (it != envmap.end() && it->second.type == ProcedureType)
    {
        std::vector<Atom> atomArgs;

        for (const auto& exp : args)
        {
            Atom atom = exp.head; // Directly use the head of the Expression as the Atom

            // Check if the Atom is of a type that needs to be converted to another Atom type
            if (atom.type == SymbolType && !token_to_atom(atom.value.sym_value, atom))
            {
                throw InterpreterSemanticError("Error: Failed to convert symbol to atom.");
            }
            else if (atom.type != NumberType && atom.type != BooleanType &&
                atom.type != PointType && atom.type != LineType && atom.type != ArcType)
            {
                // If the atom type is not one of the expected types, throw an error
                throw InterpreterSemanticError("Error: Unexpected expression type.");
            }

            atomArgs.push_back(atom);
        }

        return it->second.proc(atomArgs);
    }

    throw InterpreterSemanticError("Error: Symbol not found or not associated with a procedure.");
}
