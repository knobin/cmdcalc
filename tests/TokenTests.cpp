//
//  TokenTests.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-11-02.
//

// Local Headers
#include "calceval/Token.hpp"

// Catch2 Headers
#include "catch2/catch_all.hpp"

TEST_CASE("Constructors")
{
    SECTION("Token")
    {
        CalcEval::Token token{CalcEval::TokenType::Identifier, {20, 25}, "test"};
        REQUIRE(token.type == CalcEval::TokenType::Identifier);
        REQUIRE(token.location.line == 20);
        REQUIRE(token.location.column == 25);
        REQUIRE(token.value == "test");
    }

    SECTION("Copy")
    {
        CalcEval::Token t1{CalcEval::TokenType::Identifier, {20, 25}, "test"};
        CalcEval::Token t2{t1};
        REQUIRE(t1.type == CalcEval::TokenType::Identifier);
        REQUIRE(t1.location.line == 20);
        REQUIRE(t1.location.column == 25);
        REQUIRE(t1.value == "test");

        REQUIRE(t2.type == CalcEval::TokenType::Identifier);
        REQUIRE(t2.location.line == 20);
        REQUIRE(t2.location.column == 25);
        REQUIRE(t2.value == "test");
    }

    SECTION("Move")
    {
        CalcEval::Token t1{CalcEval::TokenType::Identifier, {20, 25}, "test"};
        CalcEval::Token t2{std::move(t1)};

        REQUIRE(t2.type == CalcEval::TokenType::Identifier);
        REQUIRE(t2.location.line == 20);
        REQUIRE(t2.location.column == 25);
        REQUIRE(t2.value == "test");
    }
}

TEST_CASE("Assignment")
{
    SECTION("Copy")
    {
        CalcEval::Token t1{CalcEval::TokenType::Identifier, {20, 25}, "test"};
        CalcEval::Token t2{CalcEval::TokenType::Number, {10, 15}, "tset"};
        t2 = t1;
        REQUIRE(t1.type == CalcEval::TokenType::Identifier);
        REQUIRE(t1.location.line == 20);
        REQUIRE(t1.location.column == 25);
        REQUIRE(t1.value == "test");

        REQUIRE(t2.type == CalcEval::TokenType::Identifier);
        REQUIRE(t2.location.line == 20);
        REQUIRE(t2.location.column == 25);
        REQUIRE(t2.value == "test");
    }

    SECTION("Move")
    {
        CalcEval::Token t1{CalcEval::TokenType::Identifier, {20, 25}, "test"};
        CalcEval::Token t2{CalcEval::TokenType::Number, {10, 15}, "tset"};
        t2 = std::move(t1);

        REQUIRE(t2.type == CalcEval::TokenType::Identifier);
        REQUIRE(t2.location.line == 20);
        REQUIRE(t2.location.column == 25);
        REQUIRE(t2.value == "test");
    }
}

TEST_CASE("Print")
{
    CalcEval::Token token{CalcEval::TokenType::Identifier, {20, 25}, "test"};
    const std::string correct{"< { token: \"" + std::string{CalcEval::tokenStr(token.type)} +
                              "\" }, { val: \"" + token.value + "\" }, { loc: { line: \"" +
                              std::to_string(token.location.line) + "\", col: \"" +
                              std::to_string(token.location.column) + "\" } }" + " >"};
    std::ostringstream oss1{};
    token.print(oss1);
    REQUIRE(oss1.str() == correct);
    std::ostringstream oss2{};
    oss2 << token;
    REQUIRE(oss2.str() == correct);
}

TEST_CASE("tokenStr")
{
    constexpr std::array<std::pair<CalcEval::TokenType, std::string_view>, 12> match{
        std::pair{CalcEval::TokenType::Bad, "bad"},
        std::pair{CalcEval::TokenType::Number, "number"},
        std::pair{CalcEval::TokenType::Identifier, "identifier"},
        std::pair{CalcEval::TokenType::Plus, "plus"},
        std::pair{CalcEval::TokenType::Minus, "minus"},
        std::pair{CalcEval::TokenType::Multiply, "multiply"},
        std::pair{CalcEval::TokenType::Divide, "divide"},
        std::pair{CalcEval::TokenType::Power, "power"},
        std::pair{CalcEval::TokenType::LeftParen, "left paren"},
        std::pair{CalcEval::TokenType::RightParen, "right paren"},
        std::pair{CalcEval::TokenType::EndMark, "end of file"},
        std::pair{CalcEval::TokenType::EndOfLine, "end of line"}};

    for (const std::pair<CalcEval::TokenType, std::string_view>& pair : match)
    {
        REQUIRE(CalcEval::tokenStr(pair.first) == pair.second);
    }
}