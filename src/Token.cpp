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
    Token::Token(TokenType type, const Location& loc, const std::string& value)
        : type{type}, value{value}, location{loc}
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

    std::string tokenStr(TokenType type)
    {
        switch (type)
        {
            case TokenType::Bad:
                return "bad";
            case TokenType::Number:
                return "number";
            case TokenType::Identifier:
                return "identifier";
            case TokenType::Plus:
                return "plus";
            case TokenType::Minus:
                return "minus";
            case TokenType::Multiply:
                return "multiply";
            case TokenType::Divide:
                return "divide";
            case TokenType::Power:
                return "power";
            case TokenType::LeftParen:
                return "left paren";
            case TokenType::RightParen:
                return "right paren";
            case TokenType::EndMark:
                return "end of file";
            case TokenType::EndOfLine:
                return "end of line";
        }

        return "<unknown>";
    }

    std::ostream& operator<<(std::ostream& os, const Token& token)
    {
        token.print(os);
        return os;
    }

} // namespace CalcEval
