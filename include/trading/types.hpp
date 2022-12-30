//
// Created by Tomáš Petříček on 01.11.2022.
//

#ifndef BACKTESTING_TYPES_HPP
#define BACKTESTING_TYPES_HPP

//#include <boost/rational.hpp>

namespace trading {
    typedef float price_t, percent_t, fraction_t, amount_t;
    typedef std::size_t index_t;

    using percent = struct percent_tag;
    using amount = struct amount_tag;
}

#endif //BACKTESTING_TYPES_HPP
