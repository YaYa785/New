#include "tokenize.hpp"
#include <cctype>

#include <iostream>

TokenSequenceType tokenize(std::istream & seq)
{
  TokenSequenceType tokens;

  char c;
  std::string token;

  // A flag to track if we're inside a comment
  bool inComment = false;

  while (seq.get(c))
  {
	  if (inComment)
	  {
		  if (c == '\n')
		  {
			  inComment = false;
		  }
	  }
	  else if (c == COMMENT)
	  {
		  inComment = true;
	  }
	  else if (c == OPEN || c == CLOSE)
	  {
		  // Handle parentheses as individual tokens
		  if (!token.empty())
		  {
			  tokens.push_back(token);
			  token.clear();
		  }
		  tokens.push_back(std::string(1, c));
	  }
	  else if (std::isspace(c) == 0)
	  {
		  // Handle non-whitespace characters
		  token += c;
	  }
	  else if (!token.empty())
	  {
		  // Handle whitespace as a separator between tokens
		  tokens.push_back(token);
		  token.clear();
	  }
  }

  // Check if there's any remaining token to add
  if (!token.empty())
  {
	  tokens.push_back(token);
  }
	
  return tokens;
}
