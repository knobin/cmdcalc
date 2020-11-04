//
//  tests/ScannerTests.hpp
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
#include <algorithm>
#include <ctime>
#include <sstream>
#include <random>
#include <iostream>

TEST_CASE("Expected input")
{
    constexpr std::array<CalcEval::TokenType, 10> sequence{
        CalcEval::TokenType::Number,     CalcEval::TokenType::Identifier,
        CalcEval::TokenType::Plus,       CalcEval::TokenType::Minus,
        CalcEval::TokenType::Multiply,   CalcEval::TokenType::Divide,
        CalcEval::TokenType::Power,      CalcEval::TokenType::LeftParen,
        CalcEval::TokenType::RightParen, CalcEval::TokenType::EndMark};

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

    SECTION("Identifier")
    {
        constexpr std::string_view expr{"ident ident123 i1den2t3"};
        std::istringstream iss{std::string{expr}};
        CalcEval::Scanner scanner{iss};

        constexpr std::array<std::string_view, 3> match{"ident", "ident123", "i1den2t3"};
        for (int i{3}; i < 3; ++i)
        {
            const CalcEval::Token token{scanner.scan()};
            REQUIRE(CalcEval::TokenType::Identifier == token.type);
            REQUIRE(match[i] == token.value);
        }
    }

    SECTION("Number")
    {
        constexpr std::string_view expr{"123 123.001"};
        std::istringstream iss{std::string{expr}};
        CalcEval::Scanner scanner{iss};

        constexpr std::array<std::string_view, 2> match{"123", "123.001"};
        for (int i{2}; i < 2; ++i)
        {
            const CalcEval::Token token{scanner.scan()};
            REQUIRE(CalcEval::TokenType::Number == token.type);
            REQUIRE(match[i] == token.value);
        }
    }
}

TEST_CASE("Current line")
{
    SECTION("One line")
    {
        std::istringstream iss{"test 1234 - * 1 +"};
        CalcEval::Scanner scanner{iss};
        CalcEval::Token token{scanner.scan()};
        REQUIRE(token.value == "test");
        token = scanner.scan();
        REQUIRE(token.value == "1234");
        REQUIRE(scanner.currentLine() == "test 1234");
    }

    SECTION("Multi line")
    {
        std::istringstream iss{"test \n 1234 \n - * 1 +"};
        CalcEval::Scanner scanner{iss};
        CalcEval::Token token{scanner.scan()};
        REQUIRE(token.value == "test");
        token = scanner.scan();
        token = scanner.scan();
        REQUIRE(token.value == "1234");
        token = scanner.scan();
        token = scanner.scan();
        REQUIRE(token.value == "-");
        token = scanner.scan();
        REQUIRE(token.value == "*");
        REQUIRE(scanner.currentLine() == " - *");
    }
}

bool valid(int c)
{
    if (std::isspace(c) || std::isalpha(c) || std::isdigit(c))
        return true;

    const char car{static_cast<char>(c)};
    constexpr std::array<char, 7> match{'+', '-', '*', '/', '^', '(', ')'};
    const auto found = std::find(match.cbegin(), match.cend(),car);
    if (found != match.cend())
        return true;

    return false;
}

int randomASCII()
{
    const unsigned int time_ui{static_cast<unsigned int>(time(nullptr))};
    static std::default_random_engine eng{time_ui};
    static std::uniform_int_distribution<int> dist{32, 126};
    return dist(eng);
}

TEST_CASE("Unexpected input")
{
    SECTION("Random characters")
    {
        for (int i{0}; i < 1000; ++i)
        {
            int c{randomASCII()};
            std::istringstream iss{std::string{static_cast<char>(c)}};
            CalcEval::Scanner scanner{iss};
            if (!valid(c))
            {
                REQUIRE_THROWS_AS(scanner.scan(), CalcEval::ScannerError);
            }
        }
    }

    SECTION("Number starting with dot")
    {
        std::istringstream iss{".0001"};
        CalcEval::Scanner scanner{iss};
        REQUIRE_THROWS_AS(scanner.scan(), CalcEval::ScannerError);
    }
}