#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

// system includes
#include <map>

// module includes
#include "expression.hpp"

Expression notProcedure(const std::vector<Atom>& args);
Expression andProcedure(const std::vector<Atom>& args);
Expression orProcedure(const std::vector<Atom>& args);
Expression ADDProcedure(const std::vector<Atom>& args);
Expression subtractProcedure(const std::vector<Atom>& args);
Expression multiplyProcedure(const std::vector<Atom>& args);
Expression divideProcedure(const std::vector<Atom>& args);
Expression lessThanProcedure(const std::vector<Atom>& args);
Expression lessThanOrEqualProcedure(const std::vector<Atom>& args);
Expression greaterThanProcedure(const std::vector<Atom>& args);
Expression greaterThanOrEqualProcedure(const std::vector<Atom>& args);
Expression equalProcedure(const std::vector<Atom>& args);
Expression log10Procedure(const std::vector<Atom>& args);
Expression powProcedure(const std::vector<Atom>& args);

class Environment
{
public:
  Environment();
  void addSymbol(const Symbol& symbol, const Expression& value);
  void addProcedure(const Symbol& symbol, Procedure procedure);
  Expression get(const Symbol& symbol);
  bool isSymbolDefined(const Symbol& symbol);
  Expression evaluateProcedure(const Symbol& symbol, const std::vector<Expression>& args);


private:

  // Environment is a mapping from symbols to expressions or procedures
  enum EnvResultType {ExpressionType, ProcedureType};
  struct EnvResult{
    EnvResultType type;
    Expression exp;
    Procedure proc;
  };

  std::map<Symbol,EnvResult> envmap;
};

#endif
