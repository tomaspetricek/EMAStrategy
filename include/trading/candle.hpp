//
// Created by Tomáš Petříček on 27.06.2022.
//

#ifndef EMASTRATEGY_CANDLE_HPP
#define EMASTRATEGY_CANDLE_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <trading/types.hpp>
#include <trading/pack.hpp>
#include <trading/currency.hpp>
#include <ostream>

namespace trading {
    class candle {
        std::time_t opened_;
        price_t open_{};
        price_t high_{};
        price_t low_{};
        price_t close_{};

    public:
        candle() = default;

        explicit candle(std::time_t opened, const price_t& open, const price_t& high,
                const price_t& low, const price_t& close)
                :opened_(opened), open_(price_t{open}), high_(price_t{high}),
                 low_(price_t{low}), close_(price_t{close})
        {
            if (high<low)
                throw std::invalid_argument("High price has to be lower or equal to low price");
        }

        // mean high and low
        static price_t hl2(const candle& candle)
        {
            return mean(candle.high_, candle.low_);
        }

        // mean open, high, low, close
        static price_t hlc3(const candle& candle)
        {
            return mean(candle.high_, candle.low_);
        }

        // mean open, high, low, close
        static price_t ohlc4(const candle& candle)
        {
            return mean(candle.open_, candle.high_, candle.low_, candle.close_);
        }

        bool risen() const
        {
            return close_<open_;
        }

        bool operator==(const candle& rhs) const
        {
            return opened_==rhs.opened_ &&
                    open_==rhs.open_ &&
                    high_==rhs.high_ &&
                    low_==rhs.low_ &&
                    close_==rhs.close_;
        }

        bool operator!=(const candle& rhs) const
        {
            return !(rhs==*this);
        }

        friend std::ostream& operator<<(std::ostream& os, const candle& candle)
        {
            os << "opened: " << candle.opened_ << " open: " << candle.open_ << " high: " << candle.high_ << " low: "
               << candle.low_ << " close: " << candle.close_;
            return os;
        }

        std::time_t opened() const
        {
            return opened_;
        }

        price_t open() const
        {
            return open_;
        }

        price_t high() const
        {
            return high_;
        }

        price_t low() const
        {
            return low_;
        }

        price_t close() const
        {
            return close_;
        }
    };
}

#endif //EMASTRATEGY_CANDLE_HPP
