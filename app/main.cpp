// CalcEval
#include "calceval/Parser.hpp"

// C++ Headers
#include <iostream>
#include <optional>
#include <sstream>

std::optional<double> parse(const std::string& expr)
{
    try
    {
        CalcEval::Parser parser{};
        return parser.parse(expr);
    }
    catch (CalcEval::ParserError& e)
    {
        std::cerr << "Error parsing!\n" << e.what() << std::endl;
    }
    catch (CalcEval::ScannerError& e)
    {
        std::cerr << "Error scanning!\n" << e.what() << std::endl;
    }

    return std::nullopt;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        // REPL
        // Every line will be evaluated on its own.
        std::cout << "> " << std::flush;
        std::string line;
        while (std::getline(std::cin, line))
        {
            if (auto val = parse(line))
            {
                std::cout << *val << std::endl;
            }
            std::cout << "> " << std::flush;
        }

        std::cout << std::endl;
    }
    else if (argc > 1)
    {
        // Evaluate each expression arguments.
        for (int i{1}; i < argc; ++i)
        {
            if (auto val = parse(argv[i]))
            {
                std::cout << *val << std::endl;
            }
        }
    }

    return 0;
}
