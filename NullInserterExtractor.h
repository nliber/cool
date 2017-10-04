#ifndef COOL_NULLINSERTEREXTRACTOR_H_
#define COOL_NULLINSERTEREXTRACTOR_H_

#include <istream>
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
//
// NullInserterExtractor
//
//  An inserter/extractor that does nothing and contains no state
//
///////////////////////////////////////////////////////////////////////////////

namespace cool
{

    class NullInserterExtractor
    {
        template<typename charT, typename traits>
        friend std::basic_istream<charT, traits>&
        operator>>(std::basic_istream<charT, traits>& is, NullInserterExtractor)
        { return is; }

        template<typename charT, typename traits>
        friend std::basic_ostream<charT, traits>&
        operator<<(std::basic_ostream<charT, traits>& os, NullInserterExtractor)
        { return os; }
    };

} // cool namespace

#endif /* COOL_NULLINSERTEREXTRACTOR_H_ */

