//
// Created by Tomáš Petříček on 02.11.2022.
//

#ifndef BACKTESTING_MOTION_TRACKER_HPP
#define BACKTESTING_MOTION_TRACKER_HPP

#include <trading/types.hpp>

namespace trading {
    struct motion {
        amount_t peak;
        amount_t trough;

        explicit motion(const amount_t& peak, const amount_t& trough)
                :peak(peak), trough(trough)
        {
            assert(peak>=trough);
        }
    };

    struct drawdown : public motion {
        // peak happens before trough
        explicit drawdown(const amount_t& peak, const amount_t& trough)
                :motion(peak, trough) { }

        template<class T>
        requires std::same_as<T, amount_t>
        amount_t value() const
        {
            assert(trough-peak<=amount_t{0});
            return trough-peak;
        }

        template<class T>
        requires std::same_as<T, percent_t>
        percent_t value() const
        {
            return percent_t{value_of((trough-peak)/peak)*100};
        }
    };

    struct run_up : public motion {
        // note: trough happens before peak
        explicit run_up(const amount_t& trough, const amount_t& peak)
                :motion(peak, trough) { }

        template<class T>
        requires std::same_as<T, amount_t>
        amount_t value() const
        {
            assert(peak-trough>=amount_t{0});
            return peak-trough;
        }

        template<class T>
        requires std::same_as<T, percent_t>
        percent_t value() const
        {
            return percent_t{value_of((peak-trough)/trough)*100};
        }
    };

    class drawdown_tracker {
        drawdown max_;
        drawdown curr_;

    public:
        typedef drawdown motion_type;

        explicit drawdown_tracker(const amount_t init)
                :max_(init, init), curr_(init, init) { }

        void update(amount_t val)
        {
            // going up -> drawdown is zero
            // moves in opposite direction -> goes with it
            if (val>curr_.peak) {
                curr_.peak = val;
                curr_.trough = val;
            }
            else if (val<curr_.trough) {
                curr_.trough = val;
            }

            if (curr_.value<amount_t>()<max_.value<amount_t>())
                max_ = curr_;
        }

        const drawdown& max() const
        {
            return max_;
        }
        const drawdown& current() const
        {
            return curr_;
        }
    };

    class run_up_tracker {
        run_up max_;
        run_up curr_;

    public:
        typedef run_up motion_type;

        explicit run_up_tracker(const amount_t init)
                :max_(init, init), curr_(init, init) { }

        void update(amount_t val)
        {
            // going down -> run up is zero
            // moves in opposite direction -> goes with it
            if (val<curr_.trough) {
                curr_.peak = val;
                curr_.trough = val;
            }
            else if (val>curr_.peak) {
                curr_.peak = val;
            }

            if (curr_.value<amount_t>()>max_.value<amount_t>())
                max_ = curr_;
        }

        const run_up& max() const
        {
            return max_;
        }
        const run_up& current() const
        {
            return curr_;
        }
    };
}

#endif //BACKTESTING_MOTION_TRACKER_HPP
