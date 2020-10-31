#include "calceval/Scanner.hpp"

// C++ Headers
#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    std::stringstream ss{"pi+pi+10+1+2"};
    CalcEval::Scanner scanner{ss};

    while (true)
    {
        CalcEval::Token token{scanner.scan()};
        std::cout << token << "\n";

        if (token.type == CalcEval::TokenType::EndMark)
            break;
    }

    return 0;
}
