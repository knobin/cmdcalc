//
//  Parser.hpp
//  CalcEval
//
//  Created by Robin Gustafsson on 2020-10-30.
//

#ifndef CALCEVAL_PARSER_HPP
#define CALCEVAL_PARSER_HPP

// Local Headers
#include "calceval/ParserLogic.hpp"
#include "calceval/type/Standard.hpp"

namespace CalcEval
{
    template<typename CalcType = Type::Standard>
    class Parser
    {
    public:
        using value_type = typename CalcType::value_type;

    public:
        Parser() = default;

        value_type parse(std::istringstream& iss) const
        {
            ParserLogic<CalcType> logic{iss};
            return logic.parse();
        }

        value_type parse(std::ifstream& ifs) const
        {
            ParserLogic<CalcType> logic{ifs};
            return logic.parse();
        }

        value_type parse(const std::string& str) const
        {
            std::istringstream iss{str};
            ParserLogic<CalcType> logic{iss};
            return logic.parse();
        }

    };

} // namespace CalcEval


#endif // CALCEVAL_PARSER_HPP
