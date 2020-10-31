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
#include <string>
#include <ostream>

namespace CalcEval
{
    struct Location
    {
        std::size_t line;
        std::size_t column;
    };

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

    class Token
    {
    public:
        Token() = delete;
        explicit Token(TokenType type, const Location& loc, std::string value = "");

        void print(std::ostream& os) const;

        const TokenType type;
        const std::string value;
        const Location location;
    };

    [[nodiscard]] std::string tokenStr(TokenType type);
    std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace CalcEval

#endif // CALCEVAL_TOKEN_HPP