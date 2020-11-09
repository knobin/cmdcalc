//
//  tests/ParserTests.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-11-02.
//

// Local Headers
#include "calceval/Parser.hpp"

// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// C++ Headers
#include <sstream>
#include <cmath>

bool closeEnough(double a, double b)
{
    return std::abs(a - b) < 1e-9;
}

bool evaluate(const std::string& expr, double expected)
{
    std::istringstream iss{expr};
    CalcEval::Parser parser{iss};
    return closeEnough(parser.parse(), expected);
}

///////////////////////////////////////////////////////////////////////////////

TEST_CASE("Numeric value")
{
    SECTION("Integer")
    {
        REQUIRE(evaluate("10", 10.0));
        REQUIRE(evaluate("-25", -25.0));
    }

    SECTION("Double")
    {
        REQUIRE(evaluate("10.0", 10.0));
        REQUIRE(evaluate("-25.25", -25.25));
    }
}

TEST_CASE("Symbolic constant")
{
    SECTION("pi")
    {
        REQUIRE(evaluate("pi", 3.14159265));
        REQUIRE(evaluate("-pi", -3.14159265));
    }

    SECTION("e")
    {
        REQUIRE(evaluate("e", 2.71828183));
        REQUIRE(evaluate("-e", -2.71828183));
    }
}

TEST_CASE("Function call")
{
    SECTION("log")
    {
        REQUIRE(evaluate("log(10)", 2.302585093));
        REQUIRE(evaluate("log(2.718281828459045)", 1.0));
    }

    SECTION("log10")
    {
        REQUIRE(evaluate("log10(10)", 1.0));
        REQUIRE(evaluate("log10(1)", 0.0));
    }

    SECTION("exp")
    {
        REQUIRE(evaluate("exp(10)", 22026.4657948067));
        REQUIRE(evaluate("exp(1)", 2.7182818285));
    }

    SECTION("sin")
    {
        REQUIRE(evaluate("sin(pi/2)", 1.0));
        REQUIRE(evaluate("sin(0)", 0.0));
    }

    SECTION("tan")
    {
        REQUIRE(evaluate("tan(pi/5)", 0.7265425269));
        REQUIRE(evaluate("tan(0)", 0.0));
    }

    SECTION("arcsin")
    {
        REQUIRE(evaluate("arcsin(1)", 1.5707963268));
        REQUIRE(evaluate("arcsin(0)", 0.0));
    }

    SECTION("arccos")
    {
        REQUIRE(evaluate("arccos(1)", 0.0));
        REQUIRE(evaluate("arccos(0)", 1.5707963268));
    }

    SECTION("arctan")
    {
        REQUIRE(evaluate("arctan(0)", 0.0));
        REQUIRE(evaluate("arctan(1)", 0.7853981634));
    }
}

TEST_CASE("Grouping")
{
    SECTION("Numeric")
    {
        REQUIRE(evaluate("(1)", 1.0));
        REQUIRE(evaluate("-(1)", -1.0));
        REQUIRE(evaluate("(1.5)", 1.5));
    }

    SECTION("Expression")
    {
        REQUIRE(evaluate("(1+2)", 3.0));
        REQUIRE(evaluate("-(2*5+3)", -13.0));
        REQUIRE(evaluate("(1.5+100*0.5)", 51.5));
    }

    SECTION("Multiple")
    {
        REQUIRE(evaluate("((((1))))", 1.0));
        REQUIRE(evaluate("-(-(-(-(-1))))", -1.0));
        REQUIRE(evaluate("(1+4)*(3-4)", -5.0));
    }
}

TEST_CASE("Addition")
{
    SECTION("Positive + Positive")
    {
        REQUIRE(evaluate("1+2", 3.0));
        REQUIRE(evaluate("20+10", 30.0));
        REQUIRE(evaluate("10.4+1.2", 11.6));
    }

    SECTION("Positive + Negative")
    {
        REQUIRE(evaluate("10+-2", 8.0));
        REQUIRE(evaluate("20+-10", 10.0));
        REQUIRE(evaluate("10.4+-1.2", 9.2));
    }

    SECTION("Negative + Positive")
    {
        REQUIRE(evaluate("-10+2", -8.0));
        REQUIRE(evaluate("-5+10", 5.0));
        REQUIRE(evaluate("-10.4+1.2", -9.2));
    }

    SECTION("Negative + Negative")
    {
        REQUIRE(evaluate("-10+-2", -12.0));
        REQUIRE(evaluate("-5+-10", -15.0));
        REQUIRE(evaluate("-10.4+-1.2", -11.6));
    }

    SECTION("Multiple")
    {
        REQUIRE(evaluate("2+3+4+5+6", 20.0));
        REQUIRE(evaluate("-5+-10+144-123+21", 27.0));
        REQUIRE(evaluate("-10.4+-1.2+123.2-12.43", 99.17));
    }
}

TEST_CASE("Subtraction")
{
    SECTION("Positive - Positive")
    {
        REQUIRE(evaluate("1-2", -1.0));
        REQUIRE(evaluate("20-10", 10.0));
        REQUIRE(evaluate("10.4-1.2", 9.2));
    }

    SECTION("Positive - Negative")
    {
        REQUIRE(evaluate("10--2", 12.0));
        REQUIRE(evaluate("20--10", 30.0));
        REQUIRE(evaluate("10.4--1.2", 11.6));
    }

    SECTION("Negative - Positive")
    {
        REQUIRE(evaluate("-10-2", -12.0));
        REQUIRE(evaluate("-5-10", -15.0));
        REQUIRE(evaluate("-10.4-1.2", -11.6));
    }

    SECTION("Negative - Negative")
    {
        REQUIRE(evaluate("-10--2", -8.0));
        REQUIRE(evaluate("-5--10", 5.0));
        REQUIRE(evaluate("-10.4--1.2", -9.2));
    }

    SECTION("Multiple")
    {
        REQUIRE(evaluate("50-3-4-5-6", 32.0));
        REQUIRE(evaluate("-5--10-144-123-21", -283.0));
        REQUIRE(evaluate("-10.4--1.2-123.2-12.43", -144.83));
    }
}

TEST_CASE("Multiplication")
{
    SECTION("Positive * Positive")
    {
        REQUIRE(evaluate("1*2", 2.0));
        REQUIRE(evaluate("20*10", 200.0));
        REQUIRE(evaluate("10.4*1.2", 12.48));
    }

    SECTION("Positive * Negative")
    {
        REQUIRE(evaluate("10*-2", -20.0));
        REQUIRE(evaluate("20*-10", -200.0));
        REQUIRE(evaluate("10.4*-1.2", -12.48));
    }

    SECTION("Negative * Positive")
    {
        REQUIRE(evaluate("-10*2", -20.0));
        REQUIRE(evaluate("-5*10", -50.0));
        REQUIRE(evaluate("-10.4*1.2", -12.48));
    }

    SECTION("Negative * Negative")
    {
        REQUIRE(evaluate("-10*-2", 20.0));
        REQUIRE(evaluate("-5*-10", 50.0));
        REQUIRE(evaluate("-10.4*-1.2", 12.48));
    }

    SECTION("Multiple")
    {
        REQUIRE(evaluate("50*3*4*5*6", 18000.0));
        REQUIRE(evaluate("-5*-10*14*3*21", 44100.0));
        REQUIRE(evaluate("-10.4*-1.2*123.2*12.43", 19111.57248));
    }
}

TEST_CASE("Division")
{
    SECTION("Positive / Positive")
    {
        REQUIRE(evaluate("1/2", 0.5));
        REQUIRE(evaluate("20/10", 2.0));
        REQUIRE(evaluate("10.4/1.2", 8.6666666667));
    }

    SECTION("Positive / Negative")
    {
        REQUIRE(evaluate("10/-2", -5.0));
        REQUIRE(evaluate("20/-10", -2.0));
        REQUIRE(evaluate("10.4/-1.2", -8.6666666667));
    }

    SECTION("Negative / Positive")
    {
        REQUIRE(evaluate("-10/2", -5.0));
        REQUIRE(evaluate("-5/10", -0.5));
        REQUIRE(evaluate("-10.4/1.2", -8.6666666667));
    }

    SECTION("Negative / Negative")
    {
        REQUIRE(evaluate("-10/-2", 5.0));
        REQUIRE(evaluate("-5/-10", 0.5));
        REQUIRE(evaluate("-10.4/-1.2", 8.6666666667));
    }

    SECTION("Multiple")
    {
        REQUIRE(evaluate("50/3/4/5/6", 0.1388888889));
        REQUIRE(evaluate("-5/-10/14/3/21", 0.0005668934));
        REQUIRE(evaluate("-10.4/-1.2/123.2/12.43", 0.0056593983));
    }
}

TEST_CASE("Exponentiation")
{
    SECTION("Positive ^ Positive")
    {
        REQUIRE(evaluate("3^2", 9.0));
        REQUIRE(evaluate("2^3", 8.0));
        REQUIRE(evaluate("10.4^1.2", 16.6126917648));
    }

    SECTION("Positive ^ Negative")
    {
        REQUIRE(evaluate("10^-2", 0.01));
        REQUIRE(evaluate("20^-2", 0.0025));
        REQUIRE(evaluate("10.4^-1.2", 0.060194941));
    }

    SECTION("Negative ^ Positive")
    {
        REQUIRE(evaluate("-10^2", -100.0));
        REQUIRE(evaluate("-5^3", -125.0));
        REQUIRE(evaluate("-10.4^1.2", -16.6126917648));
    }

    SECTION("Negative ^ Negative")
    {
        REQUIRE(evaluate("-10^-2", -0.01));
        REQUIRE(evaluate("-5^-3", -0.008));
        REQUIRE(evaluate("-10.4^-1.2", -0.060194941));
    }

    SECTION("Multiple")
    {
        REQUIRE(evaluate("20^1^2^3^4", 20.0));
        REQUIRE(evaluate("2^2^2^2", 65536.0));
        REQUIRE(evaluate("-2^2^2^2", -65536.0));
        REQUIRE(evaluate("-2^2^-2^2", -1.0442737824));
        REQUIRE(evaluate("-2^-2^-2^-2", -0.558296565));
    }
}

TEST_CASE("PEMDAS")
{
    REQUIRE(evaluate("4*(5+3)", 32.0));
    REQUIRE(evaluate("5*2^2", 20.0));
    REQUIRE(evaluate("2+5*3", 17.0));
    REQUIRE(evaluate("30/5*3", 18.0));
    REQUIRE(evaluate("3+6*2", 15.0));
    REQUIRE(evaluate("12/6*3/2", 3.0));
    REQUIRE(evaluate("20*2-(1/2)*9.8*2^2", 20.4));
    REQUIRE(evaluate("4^3^2", 262144.0));
    REQUIRE(evaluate("7+(6*5^2+3)", 160.0));
}