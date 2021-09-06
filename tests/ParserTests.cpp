//
//  tests/ParserTests.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-11-02.
//

// Local Headers
#include "calceval/Parser.hpp"

// Catch2 Headers
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_exception.hpp"
#include <catch2/matchers/catch_matchers_templated.hpp>

// C++ Headers
#include <cmath>
#include <sstream>

static double parse(const std::string& expr)
{
    CalcEval::Parser parser{};
    return parser.parse(expr);
}

///////////////////////////////////////////////////////////////////////////////

TEST_CASE("Numeric value")
{
    SECTION("Integer")
    {
        REQUIRE(parse("10") == Catch::Approx(10.0));
        REQUIRE(parse("-25") == Catch::Approx(-25.0));
    }

    SECTION("Double")
    {
        REQUIRE(parse("10.0") == Catch::Approx(10.0));
        REQUIRE(parse("-25.25") == Catch::Approx(-25.25));
    }
}

TEST_CASE("Symbolic constant")
{
    SECTION("pi")
    {
        REQUIRE(parse("pi") == Catch::Approx(3.14159265));
        REQUIRE(parse("-pi") == Catch::Approx(-3.14159265));
    }

    SECTION("e")
    {
        REQUIRE(parse("e") == Catch::Approx(2.71828183));
        REQUIRE(parse("-e") == Catch::Approx(-2.71828183));
    }
}

TEST_CASE("Function call")
{
    SECTION("log")
    {
        REQUIRE(parse("log(10)") == Catch::Approx(2.302585093));
        REQUIRE(parse("log(2.718281828459045)") == Catch::Approx(1.0));
    }

    SECTION("log10")
    {
        REQUIRE(parse("log10(10)") == Catch::Approx(1.0));
        REQUIRE(parse("log10(1)") == Catch::Approx(0.0));
    }

    SECTION("exp")
    {
        REQUIRE(parse("exp(10)") == Catch::Approx(22026.4657948067));
        REQUIRE(parse("exp(1)") == Catch::Approx(2.7182818285));
    }

    SECTION("sin")
    {
        REQUIRE(parse("sin(pi/2)") == Catch::Approx(1.0));
        REQUIRE(parse("sin(0)") == Catch::Approx(0.0));
    }

    SECTION("tan")
    {
        REQUIRE(parse("tan(pi/5)") == Catch::Approx(0.7265425269));
        REQUIRE(parse("tan(0)") == Catch::Approx(0.0));
    }

    SECTION("arcsin")
    {
        REQUIRE(parse("arcsin(1)") == Catch::Approx(1.5707963268));
        REQUIRE(parse("arcsin(0)") == Catch::Approx(0.0));
    }

    SECTION("arccos")
    {
        REQUIRE(parse("arccos(1)") == Catch::Approx(0.0));
        REQUIRE(parse("arccos(0)") == Catch::Approx(1.5707963268));
    }

    SECTION("arctan")
    {
        REQUIRE(parse("arctan(0)") == Catch::Approx(0.0));
        REQUIRE(parse("arctan(1)") == Catch::Approx(0.7853981634));
    }
}

TEST_CASE("Grouping")
{
    SECTION("Numeric")
    {
        REQUIRE(parse("(1)") == Catch::Approx(1.0));
        REQUIRE(parse("-(1)") == Catch::Approx(-1.0));
        REQUIRE(parse("(1.5)") == Catch::Approx(1.5));
    }

    SECTION("Expression")
    {
        REQUIRE(parse("(1+2)") == Catch::Approx(3.0));
        REQUIRE(parse("-(2*5+3)") == Catch::Approx(-13.0));
        REQUIRE(parse("(1.5+100*0.5)") == Catch::Approx(51.5));
    }

    SECTION("Multiple")
    {
        REQUIRE(parse("((((1))))") == Catch::Approx(1.0));
        REQUIRE(parse("-(-(-(-(-1))))") == Catch::Approx(-1.0));
        REQUIRE(parse("(1+4)*(3-4)") == Catch::Approx(-5.0));
    }
}

TEST_CASE("Addition")
{
    SECTION("Positive + Positive")
    {
        REQUIRE(parse("1+2") == Catch::Approx(3.0));
        REQUIRE(parse("20+10") == Catch::Approx(30.0));
        REQUIRE(parse("10.4+1.2") == Catch::Approx(11.6));
    }

    SECTION("Positive + Negative")
    {
        REQUIRE(parse("10+-2") == Catch::Approx(8.0));
        REQUIRE(parse("20+-10") == Catch::Approx(10.0));
        REQUIRE(parse("10.4+-1.2") == Catch::Approx(9.2));
    }

    SECTION("Negative + Positive")
    {
        REQUIRE(parse("-10+2") == Catch::Approx(-8.0));
        REQUIRE(parse("-5+10") == Catch::Approx(5.0));
        REQUIRE(parse("-10.4+1.2") == Catch::Approx(-9.2));
    }

    SECTION("Negative + Negative")
    {
        REQUIRE(parse("-10+-2") == Catch::Approx(-12.0));
        REQUIRE(parse("-5+-10") == Catch::Approx(-15.0));
        REQUIRE(parse("-10.4+-1.2") == Catch::Approx(-11.6));
    }

    SECTION("Multiple")
    {
        REQUIRE(parse("2+3+4+5+6") == Catch::Approx(20.0));
        REQUIRE(parse("-5+-10+144-123+21") == Catch::Approx(27.0));
        REQUIRE(parse("-10.4+-1.2+123.2-12.43") == Catch::Approx(99.17));
    }
}

TEST_CASE("Subtraction")
{
    SECTION("Positive - Positive")
    {
        REQUIRE(parse("1-2") == Catch::Approx(-1.0));
        REQUIRE(parse("20-10") == Catch::Approx(10.0));
        REQUIRE(parse("10.4-1.2") == Catch::Approx(9.2));
    }

    SECTION("Positive - Negative")
    {
        REQUIRE(parse("10--2") == Catch::Approx(12.0));
        REQUIRE(parse("20--10") == Catch::Approx(30.0));
        REQUIRE(parse("10.4--1.2") == Catch::Approx(11.6));
    }

    SECTION("Negative - Positive")
    {
        REQUIRE(parse("-10-2") == Catch::Approx(-12.0));
        REQUIRE(parse("-5-10") == Catch::Approx(-15.0));
        REQUIRE(parse("-10.4-1.2") == Catch::Approx(-11.6));
    }

    SECTION("Negative - Negative")
    {
        REQUIRE(parse("-10--2") == Catch::Approx(-8.0));
        REQUIRE(parse("-5--10") == Catch::Approx(5.0));
        REQUIRE(parse("-10.4--1.2") == Catch::Approx(-9.2));
    }

    SECTION("Multiple")
    {
        REQUIRE(parse("50-3-4-5-6") == Catch::Approx(32.0));
        REQUIRE(parse("-5--10-144-123-21") == Catch::Approx(-283.0));
        REQUIRE(parse("-10.4--1.2-123.2-12.43") == Catch::Approx(-144.83));
    }
}

TEST_CASE("Multiplication")
{
    SECTION("Positive * Positive")
    {
        REQUIRE(parse("1*2") == Catch::Approx(2.0));
        REQUIRE(parse("20*10") == Catch::Approx(200.0));
        REQUIRE(parse("10.4*1.2") == Catch::Approx(12.48));
    }

    SECTION("Positive * Negative")
    {
        REQUIRE(parse("10*-2") == Catch::Approx(-20.0));
        REQUIRE(parse("20*-10") == Catch::Approx(-200.0));
        REQUIRE(parse("10.4*-1.2") == Catch::Approx(-12.48));
    }

    SECTION("Negative * Positive")
    {
        REQUIRE(parse("-10*2") == Catch::Approx(-20.0));
        REQUIRE(parse("-5*10") == Catch::Approx(-50.0));
        REQUIRE(parse("-10.4*1.2") == Catch::Approx(-12.48));
    }

    SECTION("Negative * Negative")
    {
        REQUIRE(parse("-10*-2") == Catch::Approx(20.0));
        REQUIRE(parse("-5*-10") == Catch::Approx(50.0));
        REQUIRE(parse("-10.4*-1.2") == Catch::Approx(12.48));
    }

    SECTION("Multiple")
    {
        REQUIRE(parse("50*3*4*5*6") == Catch::Approx(18000.0));
        REQUIRE(parse("-5*-10*14*3*21") == Catch::Approx(44100.0));
        REQUIRE(parse("-10.4*-1.2*123.2*12.43") == Catch::Approx(19111.57248));
    }
}

TEST_CASE("Division")
{
    SECTION("Positive / Positive")
    {
        REQUIRE(parse("1/2") == Catch::Approx(0.5));
        REQUIRE(parse("20/10") == Catch::Approx(2.0));
        REQUIRE(parse("10.4/1.2") == Catch::Approx(8.6666666667));
    }

    SECTION("Positive / Negative")
    {
        REQUIRE(parse("10/-2") == Catch::Approx(-5.0));
        REQUIRE(parse("20/-10") == Catch::Approx(-2.0));
        REQUIRE(parse("10.4/-1.2") == Catch::Approx(-8.6666666667));
    }

    SECTION("Negative / Positive")
    {
        REQUIRE(parse("-10/2") == Catch::Approx(-5.0));
        REQUIRE(parse("-5/10") == Catch::Approx(-0.5));
        REQUIRE(parse("-10.4/1.2") == Catch::Approx(-8.6666666667));
    }

    SECTION("Negative / Negative")
    {
        REQUIRE(parse("-10/-2") == Catch::Approx(5.0));
        REQUIRE(parse("-5/-10") == Catch::Approx(0.5));
        REQUIRE(parse("-10.4/-1.2") == Catch::Approx(8.6666666667));
    }

    SECTION("Multiple")
    {
        REQUIRE(parse("50/3/4/5/6") == Catch::Approx(0.1388888889));
        REQUIRE(parse("-5/-10/14/3/21") == Catch::Approx(0.0005668934));
        REQUIRE(parse("-10.4/-1.2/123.2/12.43") == Catch::Approx(0.0056593983));
    }
}

TEST_CASE("Exponentiation")
{
    SECTION("Positive ^ Positive")
    {
        REQUIRE(parse("3^2") == Catch::Approx(9.0));
        REQUIRE(parse("2^3") == Catch::Approx(8.0));
        REQUIRE(parse("10.4^1.2") == Catch::Approx(16.6126917648));
    }

    SECTION("Positive ^ Negative")
    {
        REQUIRE(parse("10^-2") == Catch::Approx(0.01));
        REQUIRE(parse("20^-2") == Catch::Approx(0.0025));
        REQUIRE(parse("10.4^-1.2") == Catch::Approx(0.060194941));
    }

    SECTION("Negative ^ Positive")
    {
        REQUIRE(parse("-10^2") == Catch::Approx(-100.0));
        REQUIRE(parse("-5^3") == Catch::Approx(-125.0));
        REQUIRE(parse("-10.4^1.2") == Catch::Approx(-16.6126917648));
    }

    SECTION("Negative ^ Negative")
    {
        REQUIRE(parse("-10^-2") == Catch::Approx(-0.01));
        REQUIRE(parse("-5^-3") == Catch::Approx(-0.008));
        REQUIRE(parse("-10.4^-1.2") == Catch::Approx(-0.060194941));
    }

    SECTION("Multiple")
    {
        REQUIRE(parse("20^1^2^3^4") == Catch::Approx(20.0));
        REQUIRE(parse("2^2^2^2") == Catch::Approx(65536.0));
        REQUIRE(parse("-2^2^2^2") == Catch::Approx(-65536.0));
        REQUIRE(parse("-2^2^-2^2") == Catch::Approx(-1.0442737824));
        REQUIRE(parse("-2^-2^-2^-2") == Catch::Approx(-0.558296565));
    }
}

TEST_CASE("PEMDAS")
{
    REQUIRE(parse("4*(5+3)") == Catch::Approx(32.0));
    REQUIRE(parse("5*2^2") == Catch::Approx(20.0));
    REQUIRE(parse("2+5*3") == Catch::Approx(17.0));
    REQUIRE(parse("30/5*3") == Catch::Approx(18.0));
    REQUIRE(parse("3+6*2") == Catch::Approx(15.0));
    REQUIRE(parse("12/6*3/2") == Catch::Approx(3.0));
    REQUIRE(parse("20*2-(1/2)*9.8*2^2") == Catch::Approx(20.4));
    REQUIRE(parse("4^3^2") == Catch::Approx(262144.0));
    REQUIRE(parse("7+(6*5^2+3)") == Catch::Approx(160.0));
}

///////////////////////////////////////////////////////////////////////////////

class ParseErrorMatcher : public Catch::Matchers::MatcherGenericBase
{
public:
    ParseErrorMatcher(const CalcEval::Token& token, const std::string msg)
        : m_token{token}, m_msg{msg}
    {}

    bool match(const CalcEval::ParserError& in) const
    {
        return ( (in.token() == m_token) && (in.what() == m_msg) );
    }

    std::string describe() const override
    {
        std::ostringstream oss{};
        oss << m_token;
        return "Equals: " + oss.str() + " and " + m_msg;
    }

private:
    const CalcEval::Token m_token;
    const std::string m_msg;
};

ParseErrorMatcher EqualsPE(const CalcEval::Token& token, const std::string& msg)
{
    return ParseErrorMatcher{token, msg};
}

TEST_CASE("Unexpected input")
{
    SECTION("Symbol")
    {
        const std::string msg1{"Unexpected symbol at line 1 : 1\n?\n^"};
        REQUIRE_THROWS_MATCHES(parse("?"), CalcEval::ScannerError, Catch::Matchers::Message(msg1));

        const std::string msg2{"Unexpected symbol at line 1 : 2\n2?\n-^"};
        REQUIRE_THROWS_MATCHES(parse("2?2"), CalcEval::ScannerError,
                               Catch::Matchers::Message(msg2));

        const std::string msg3{"Unexpected symbol at line 1 : 2\n2$\n-^"};
        REQUIRE_THROWS_MATCHES(parse("2$2"), CalcEval::ScannerError,
                               Catch::Matchers::Message(msg3));

        const std::string msg4{"Unexpected symbol at line 1 : 1\n&\n^"};
        REQUIRE_THROWS_MATCHES(parse("&2"), CalcEval::ScannerError, Catch::Matchers::Message(msg4));
    }

    SECTION("Parse")
    {
        const std::string msg1{"Unexpected token of \"end of file\" at line 1 : "
                               "3\n2+\n--^\nExpected '(', identifier or number!"};
        const CalcEval::Token t1{CalcEval::TokenType::EndMark, {1, 3}, ""};
        REQUIRE_THROWS_MATCHES(parse("2+"), CalcEval::ParserError, EqualsPE(t1, msg1));

        const std::string msg2{"Unexpected token of \"plus\" at line 1 : "
                               "1\n+\n^\nExpected '(', identifier or number!"};
        const CalcEval::Token t2{CalcEval::TokenType::Plus, {1, 1}, "+"};
        REQUIRE_THROWS_MATCHES(parse("+2"), CalcEval::ParserError, EqualsPE(t2, msg2));

        const std::string msg3{"Unexpected token of \"plus\" at line 1 : "
                               "3\n2++\n--^\nExpected '(', identifier or number!"};
        const CalcEval::Token t3{CalcEval::TokenType::Plus, {1, 3}, "+"};
        REQUIRE_THROWS_MATCHES(parse("2++2"), CalcEval::ParserError, EqualsPE(t3, msg3));

        const std::string msg4{"Unexpected token of \"minus\" at line 1 : "
                               "2\n--\n-^\nExpected '(', identifier or number!"};
        const CalcEval::Token t4{CalcEval::TokenType::Minus, {1, 2}, "-"};
        REQUIRE_THROWS_MATCHES(parse("--3"), CalcEval::ParserError, EqualsPE(t4, msg4));
    }
}
