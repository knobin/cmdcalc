#include "calceval/Parser.hpp"

// C++ Headers
#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    std::cout << "========== Parse args ==========" << std::endl;

    // The arguments are treated as one expression per line here.
    // Although, expressions can be multiline as the parser ignores line breaks.
    for (int i{1}; i < argc; ++i)
    {
        std::stringstream expr{argv[i]};
        CalcEval::Parser p{expr};
        try
        {
            double val = p.parse();
            std::cout << "Parsed \"" << argv[i] << "\" = " << val << std::endl;
        }
        catch (CalcEval::ParserError& e)
        {
            std::cerr << "Error parsing \"" << argv[i] << "\"!\n" << e.what() << std::endl;
        }
    }

    std::cout << "========== Parse str ==========" << std::endl;

    std::stringstream ss{"pi*pi+10+1+2"};
    CalcEval::Parser parser{ss};
    try
    {
        double val = parser.parse();
        std::cout << "Parsed \"pi*pi+10+1+2\" = " << val << std::endl;
    }
    catch (CalcEval::ParserError& e)
    {
        std::cerr << "Parse error! \n" << e.what() << std::endl;
    }

    return 0;
}
