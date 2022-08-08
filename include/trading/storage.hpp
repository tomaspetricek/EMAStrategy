//
// Created by Tomáš Petříček on 08.08.2022.
//

#ifndef EMASTRATEGY_STORAGE_HPP
#define EMASTRATEGY_STORAGE_HPP

#include <trading/trade.hpp>

namespace trading {
    class storage {
        std::vector<trade> closed_;

    public:
        void save_closed_trade(const trade& closed)
        {
            closed_.emplace_back(closed);
        }

        std::vector<trade> retrieve_closed_trades()
        {
            return std::move(closed_);
        }
    };
}

#endif //EMASTRATEGY_STORAGE_HPP
