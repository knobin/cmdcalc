//
//  ParserTests.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-11-02.
//

// Local Headers
#include "calceval/Parser.hpp"

// Catch2 Headers
#include "catch2/catch_all.hpp"

TEST_CASE("Constructors")
{
    std::istringstream iss{""};
    CalcEval::Parser parser{iss};
}