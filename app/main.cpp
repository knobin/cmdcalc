// CalcEval
#include "calceval/Parser.hpp"

// C++ Headers
#include <iostream>
#include <sstream>


void parse(CalcEval::Parser parser, const std::string& expr)
{
    try
    {
        double val = parser.parse();
        std::cout << "Parsed \"" << expr << "\" = " << val << std::endl;
    }
    catch (CalcEval::ParserError& e)
    {
        std::cerr << "Error parsing \"" << expr << "\"!\n" << e.what() << std::endl;
    }
    catch (CalcEval::ScannerError& e)
    {
        std::cerr << "Error scanning \"" << expr << "\"!\n" << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    std::cout << "========== Parse args ==========" << std::endl;

    // The arguments are treated as one expression per line here.
    // Although, expressions can be multiline as the parser ignores line breaks.
    for (int i{1}; i < argc; ++i)
    {
        std::stringstream expr{argv[i]};
        CalcEval::Parser p1{expr};
        parse(p1, expr.str());
    }

    std::cout << "========== Parse str ==========" << std::endl;

    std::string toParse{"pi*pi+1.0000001+10+1+2+1.0000001"};
    std::stringstream ss{toParse};
    CalcEval::Parser p2{ss};
    parse(p2, toParse);

    return 0;
}
