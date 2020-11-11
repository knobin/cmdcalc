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
    Token::Token() : value{""}, location{-1, -1}, type{TokenType::Bad}
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

    bool operator==(const Location& lhs, const Location& rhs)
    {
        return ((lhs.line == rhs.line) && (lhs.column == rhs.column));
    }

    bool operator!=(const Location& lhs, const Location& rhs)
    {
        return !(lhs == rhs);
    }

    bool operator==(const Token& lhs, const Token& rhs)
    {
        return ((lhs.value == rhs.value) && (lhs.location == rhs.location) &&
                (lhs.type == rhs.type));
    }

    bool operator!=(const Token& lhs, const Token& rhs)
    {
        return !(lhs == rhs);
    }

} // namespace CalcEval
