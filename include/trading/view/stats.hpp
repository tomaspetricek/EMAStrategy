//
// Created by Tomáš Petříček on 26.10.2022.
//

#ifndef BACKTESTING_VIEW_STATS_HPP
#define BACKTESTING_VIEW_STATS_HPP

#include <trading/types.hpp>
#include <trading/exception.hpp>

namespace trading::view {
    // if gross loss is zero the behaviour is undefined
    inline double profit_factor(amount_t gross_profit, amount_t gross_loss)
    {
        assert(gross_loss!=amount_t{0.0});
        return gross_profit/gross_loss;
    }
}

#endif //BACKTESTING_VIEW_STATS_HPP
