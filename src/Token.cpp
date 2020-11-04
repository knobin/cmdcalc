//
//  Token.cpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

// Local Headers
#include "calceval/Token.hpp"

namespace CalcEval
{
    Token::Token()
        : value{""}, location{-1, -1}, type{TokenType::Bad}
    {
    }

    Token::Token(TokenType type, const Location& loc, const std::string& value)
        : value{value}, location{loc}, type{type}
    {
    }

    void Token::print(std::ostream& os) const
    {
        os << "< { token: \"" << tokenStr(type) << "\" }, { val: \"" << value
           << "\" }, { loc: { line: \"" << location.line << "\", col: \"" << location.column
           << "\" } }"
           << " >";
    }

    ///////////////////////////////////////////////////////////////////////////////

    std::ostream& operator<<(std::ostream& os, const Token& token)
    {
        token.print(os);
        return os;
    }

} // namespace CalcEval
