//
//  tests/OrderTests.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2021-07-23.
//

// Local Headers
#include "calceval/Parser.hpp"

// Catch2 Headers
#include "catch2/catch_test_macros.hpp"

// C++ Headers
#include <array>
#include <sstream>
#include <string_view>

///////////////////////////////////////////////////////////////////////////////

double Parse(const std::string_view& expr)
{
    std::string str{expr};
    std::istringstream iss{str};
    CalcEval::Parser parser{iss};
    return parser.parse();
}

///////////////////////////////////////////////////////////////////////////////

TEST_CASE("Order of Operations")
{
    constexpr std::array<std::pair<std::string_view, double>, 30> match{
        std::pair{"14+18/2*18-7", 169.0},   std::pair{"15*18+12/3+9", 283.0},
        std::pair{"8*4+9-9+18", 50.0},      std::pair{"11*11-6*17+4", 23.0},
        std::pair{"2-1+5*4*11", 221.0},     std::pair{"16*7*15+11+17", 1708.0}, 
        std::pair{"10-9*24/8*6", -152.0},   std::pair{"10/5+10-9*11", -87.0},
        std::pair{"3*19*14+18/2", 807},     std::pair{"10*12-14/2+15", 128},

        std::pair{"14/2-1+3", 9.0},         std::pair{"9+15/5*13", 48.0},
        std::pair{"12/3*12+10", 58.0},      std::pair{"16*15/5+12", 60.0},
        std::pair{"2*10+10-8", 22.0},       std::pair{"24/4+14*2", 34.0},
        std::pair{"11*10-12/3", 106.0},     std::pair{"8/4*2+18", 22.0},
        std::pair{"18/6+4*15", 63.0},       std::pair{"2-20/5*3", -10.0},

        std::pair{"(6+4)^2+(11+10/2)", 116.0},         std::pair{"(11+42-5)/(11-3)", 6.0},
        std::pair{"(17-3)*(14-6)-22", 90.0},      std::pair{"(9+33-6)/6-3^2", -3.0},
        std::pair{"(10+43-5)/6+5^2", 33.0},       std::pair{"2*(9*5+3^2)+4", 112.0},
        std::pair{"(6+3)^2+(9-10/5)", 88.0},     std::pair{"(10+59-3^2)/(24-4)", 3.0},
        std::pair{"4*(12*6-4^2)+9", 233.0},       std::pair{"(19-8)*(10+4)+8^2", 218.0}
    };

    for (auto& [equation, correctAnswer] : match)
    {
        REQUIRE(Parse(equation) == correctAnswer);
    }
}
