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
    CalcEval::Token token{CalcEval::TokenType::Bad, {}, ""};
}