//
//  tests/CustomImplTests.cpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2021-09-01.
//

// Local Headers
#include "calceval/Parser.hpp"

// Catch2 Headers
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_exception.hpp"

///////////////////////////////////////////////////////////////////////////////

    struct CustomImpl : public CalcEval::Type::Base<double>
    {
        [[nodiscard]] double stot(const std::string& str) noexcept
        {
            return std::stod(str);
        }

        [[nodiscard]] std::optional<double> constant(const std::string& str) noexcept
        {
            if (str == "pi")
                return 3.14159265;
            else if (str == "c1")
                return 1.23;

            return std::nullopt;
        }

        [[nodiscard]] std::optional<std::function<double(double)>> function(const std::string& str) noexcept
        {
            const std::map<std::string_view, std::function<double(double)>> funcs {
                std::pair{"log", [](double x) { return std::log(x); }},
                std::pair{"exp", [](double x) { return std::exp(x); }},
                std::pair{"div10", [](double x) { return x / 10.0; }}
            };

            const auto search = funcs.find(str);

            if (search != funcs.cend())
                return search->second;

            return std::nullopt;
        }
    };

///////////////////////////////////////////////////////////////////////////////

static double parse(const std::string& expr)
{
    CalcEval::Parser<CustomImpl> parser{};
    return parser.parse(expr);
}

///////////////////////////////////////////////////////////////////////////////

TEST_CASE("Constants")
{
    SECTION("Exists")
    {
        REQUIRE(parse("pi") == Catch::Approx(3.14159265));
        REQUIRE(parse("c1") == Catch::Approx(1.23));
    }

    SECTION("Does not exist")
    {
        REQUIRE(parse("pi") == Catch::Approx(3.14159265));
    }
}
