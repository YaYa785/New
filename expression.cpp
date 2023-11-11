#include "expression.hpp"


// system includes
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>
#include <tuple>
#include <iostream>

Expression::Expression(bool tf)
{
	head.type = BooleanType;
	head.value.bool_value = tf;
}

Expression::Expression(double num)
{
	head.type = NumberType;
	head.value.num_value = num;
}

Expression::Expression(const std::string & sym)
{
	head.type = SymbolType;
	head.value.sym_value = sym;
}

Expression::Expression(std::tuple<double,double> value)
{
	head.type = PointType;
	head.value.point_value.x = std::get<0>(value);
	head.value.point_value.y = std::get<1>(value);
}

Expression::Expression(std::tuple<double,double> start,  std::tuple<double,double> end)
{
	head.type = LineType;
	head.value.line_value.first.x = std::get<0>(start);
	head.value.line_value.first.y = std::get<1>(start);
	head.value.line_value.second.x = std::get<0>(end);
	head.value.line_value.second.y = std::get<1>(end);
}


Expression::Expression(std::tuple<double,double> center, std::tuple<double,double> start, double angle)
{
	head.type = ArcType;
	head.value.arc_value.center.x = std::get<0>(center);
	head.value.arc_value.center.y = std::get<1>(center);
	head.value.arc_value.start.x = std::get<0>(start);
	head.value.arc_value.start.y = std::get<1>(start);
	head.value.arc_value.span = angle;
}

bool Expression::operator==(const Expression & exp) const noexcept
{
	// Compare types
	if (head.type != exp.head.type)
	{
		return false;
	}

	// Check if both expressions have NoneType heads
	if (head.type == NoneType && exp.head.type == NoneType)
	{
		return true;
	}

	switch (head.type)
	{
	case BooleanType:
		return head.value.bool_value == exp.head.value.bool_value;
	case NumberType:
		// Compare floating-point numbers with tolerance
		return std::abs(head.value.num_value - exp.head.value.num_value) <= std::numeric_limits<double>::epsilon();
	case SymbolType:
		return head.value.sym_value == exp.head.value.sym_value;
	case PointType:
		return head.value.point_value == exp.head.value.point_value;
	case LineType:
		return (head.value.line_value.first == exp.head.value.line_value.first) &&
			(head.value.line_value.second == exp.head.value.line_value.second);
	case ArcType:
		return (head.value.arc_value.center == exp.head.value.arc_value.center) &&
			(head.value.arc_value.start == exp.head.value.arc_value.start) &&
			(fabs(head.value.arc_value.span - exp.head.value.arc_value.span) < std::numeric_limits<double>::epsilon());
	default:
		std::cerr << "ERROR: Invalid type " << std::endl;
		return false; // Invalid type
	}
	return true;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp)
{
	if (exp.tail.empty())
	{
		if (exp.head.type == BooleanType)
		{
			out << (exp.head.value.bool_value ? "True" : "False");
		}
		else if (exp.head.type == NumberType)
		{
			out << exp.head.value.num_value;
		}
		else if (exp.head.type == SymbolType)
		{
			out << exp.head.value.sym_value;
		}
		else if (exp.head.type == PointType) 
		{
			out << "(" << exp.head.value.point_value.x << "," << exp.head.value.point_value.y << ")";
		}
		else if (exp.head.type == LineType) 
		{
			out << "((" << exp.head.value.line_value.first.x << "," << exp.head.value.line_value.first.y << ")," << "(" << exp.head.value.line_value.second.x << "," << exp.head.value.line_value.second.y << "))";
		}
		else if (exp.head.type == ArcType) 
		{
			out << "((" << exp.head.value.arc_value.center.x << "," << exp.head.value.arc_value.center.y << ")," << "(" << exp.head.value.arc_value.start.x << "," << exp.head.value.arc_value.start.y << ")," << exp.head.value.arc_value.span << ")";
		}
	}
	else
	{
		out << "(" << exp.head;

		for (const Expression& tailExpr : exp.tail)
		{
			out << " " << tailExpr;
		}

		out << ")";
	}

	return out;
}

bool token_to_atom(const std::string & token, Atom & atom)
{
	if (token.empty() || token == " ")
	{
		return false;
	}

	if (token == "True")
	{
		atom.type = BooleanType;
		atom.value.bool_value = true;
	}
	else if (token == "False")
	{
		atom.type = BooleanType;
		atom.value.bool_value = false;
	}
	else
	{
		// Attempt to parse the token as a double, checking for invalid characters
		try
		{
			std::size_t pos = 0; // Position of the character following the last character interpreted
			double num = std::stod(token, &pos);

			// Check if the entire string was consumed, with no leftover characters
			if (pos != token.size())
			{
				// If not all characters were used, it's an invalid number token
				throw std::invalid_argument("");
			}

			atom.type = NumberType;
			atom.value.num_value = num;
		}
		catch (const std::invalid_argument&)
		{
			// If it's not a valid number, then it's a symbol
			// But first, we need to ensure it's a valid symbol (e.g., doesn't start with a digit or isn't a floating point)
			if (std::isdigit(token[0]) == 0 && token.find('.') == std::string::npos)
			{
				atom.type = SymbolType;
				atom.value.sym_value = token;
			}
			else
			{
				return false; // Invalid token
			}
		}
		catch (const std::out_of_range&)
		{
			// The number is out of the range of representable values by a double
			return false;
		}
	}
	return true; // Valid token
}

