//
//  ScannerTests.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-11-02.
//

// Local Headers
#include "calceval/Scanner.hpp"

// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// C++ Headers
#include <array>
#include <sstream>

TEST_CASE("Expected input")
{
    constexpr std::array<CalcEval::TokenType, 10> sequence{
        CalcEval::TokenType::Number,
        CalcEval::TokenType::Identifier,
        CalcEval::TokenType::Plus,
        CalcEval::TokenType::Minus,
        CalcEval::TokenType::Multiply,
        CalcEval::TokenType::Divide,
        CalcEval::TokenType::Power,
        CalcEval::TokenType::LeftParen,
        CalcEval::TokenType::RightParen,
        CalcEval::TokenType::EndMark
    };

    SECTION("No whitespaces")
    {
        constexpr std::string_view expr{"1.0id+-*/^()"};
        std::istringstream iss{std::string{expr}};
        CalcEval::Scanner scanner{iss};
        for (CalcEval::TokenType type : sequence)
        {
            const CalcEval::Token token{scanner.scan()};
            REQUIRE(type == token.type);
        }
    }

    SECTION("With whitespaces")
    {
        constexpr std::string_view expr{" 1.0 id + - * / ^ ( ) "};
        std::istringstream iss{std::string{expr}};
        CalcEval::Scanner scanner{iss};
        for (CalcEval::TokenType type : sequence)
        {
            const CalcEval::Token token{scanner.scan()};
            REQUIRE(type == token.type);
        }
    }
}

TEST_CASE("Unexpected input")
{

}