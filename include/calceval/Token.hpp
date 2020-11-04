//
//  Token.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

#ifndef CALCEVAL_TOKEN_HPP
#define CALCEVAL_TOKEN_HPP

// C++ Headers
#include <cstdint>
#include <ostream>
#include <string>

namespace CalcEval
{
    /** Location struct implementation.

        line and column numbers.
    */
    struct Location
    {
        using value_type = int32_t;

        value_type line;
        value_type column;
    };

    /** TokenType enum class implementation.

        Types that the Token can be and what is
        stored in the token value.
    */
    enum class TokenType : uint8_t
    {
        Bad,
        Number,
        Identifier,
        Plus,
        Minus,
        Multiply,
        Divide,
        Power,
        LeftParen,
        RightParen,
        EndMark,
        EndOfLine
    };

    /** Token class implementation.

        Types that the Token can be and what is
        stored in the token value.
    */
    class Token
    {
    public:
        /** Default Token constructor .

            type = TokenType::Bad
            location = {-1, -1}
            value = ""

            @return     default initialized Token
        */
        Token();

        /** Default Token constructor with type, loc and value.

            @param  type    type of the token
            @param  loc     location where it was found
            @param  value   token value
            @return         default initialized Token
        */
        Token(TokenType type, const Location& loc, const std::string& value);

        /** Function for printing the token.

            @param os
        */
        void print(std::ostream& os) const;

        std::string value;
        Location location;
        TokenType type;
    };

    /** Function for converting the TokenType to a string format.

        @param  type    TokenType to convert
        @return         string format of TokenType
    */
    [[nodiscard]] constexpr std::string_view tokenStr(TokenType type)
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

    /** operator<< overload to print the Token.

        @param os       ostream to output to
        @param token    token to print
        @return         ostream
    */
    std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace CalcEval

#endif // CALCEVAL_TOKEN_HPP
