//
// Created by Tomáš Petříček on 29.10.2022.
//

#ifndef BACKTESTING_RESAMPLER_HPP
#define BACKTESTING_RESAMPLER_HPP

#include <cstddef>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <trading/types.hpp>
#include <trading/candle.hpp>

namespace trading {
    class resampler {
        size_t period_{1};
        std::time_t opened_{};
        price_t open_, high_, low_, close_;
        std::size_t count_{0};

        static size_t validate_period(size_t period)
        {
            if (!period)
                throw std::invalid_argument("Resampling period has to be greater than 0");
            return period;
        }

    public:
        resampler() = default;

        explicit resampler(size_t period)
                :period_(validate_period(period)) { }

        // Returns if the output candle is ready
        bool operator()(const candle& in, candle& out)
        {
            count_++;
            bool ready{false};

            if (count_%period_==1) {
                opened_ = in.opened();
                open_ = in.open();
                low_ = in.low();
                high_ = in.high();
            }
            else {
                low_ = std::min(low_, in.low());
                high_ = std::max(high_, in.high());
            }

            if (count_-1 && count_%period_==0) {
                close_ = in.close();
                out = candle{opened_, open_, high_, low_, close_};
                ready = true;
            }

            return ready;
        }

        std::size_t period() const
        {
            return period_;
        }
    };
}

#endif //BACKTESTING_RESAMPLER_HPP
