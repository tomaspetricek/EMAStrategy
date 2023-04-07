//
// Created by Tomáš Petříček on 04.09.2022.
//

#ifndef BACKTESTING_ORDER_HPP
#define BACKTESTING_ORDER_HPP

#include <ostream>
#include <boost/date_time/posix_time/ptime.hpp>
#include <trading/types.hpp>

namespace trading {
    struct open_order {
        amount_t sold;
        price_t price;
        std::time_t created;
    };

    struct close_all_order {
        price_t price;
        std::time_t created;
    };
}

#endif //BACKTESTING_ORDER_HPP
