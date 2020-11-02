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
#include <sstream>

TEST_CASE("Constructors")
{
    std::istringstream iss{""};
    CalcEval::Scanner scanner{iss};
}