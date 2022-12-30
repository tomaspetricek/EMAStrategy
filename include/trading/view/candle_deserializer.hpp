//
// Created by Tomáš Petříček on 20.08.2022.
//

#ifndef BACKTESTING_CANDLE_DESERIALIZER_HPP
#define BACKTESTING_CANDLE_DESERIALIZER_HPP

#include <trading/candle.hpp>

namespace trading::view {
    auto candle_deserializer = [](std::time_t opened, price_t open, price_t high, price_t low, price_t close) {
        if (opened<0)
            throw std::invalid_argument("Candle time has to greater or equal to 0");

        if (open<0 || high<0 || low<0 || close<0)
            throw std::invalid_argument("Price has to be greater or equal to 0");

        // create candle
        try {
            return trading::candle(opened, open, high, low, close);
        }
        catch (...) {
            std::throw_with_nested(std::runtime_error("Cannot create candle"));
        }
    };
}

#endif //BACKTESTING_CANDLE_DESERIALIZER_HPP
