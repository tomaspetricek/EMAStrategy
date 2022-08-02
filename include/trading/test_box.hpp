//
// Created by Tomáš Petříček on 01.07.2022.
//

#ifndef EMASTRATEGY_TEST_BOX_HPP
#define EMASTRATEGY_TEST_BOX_HPP

#include <utility>

#include <trading/exception.hpp>
#include <trading/indicator/ema.hpp>
#include <trading/currency.hpp>
#include <trading/position.hpp>
#include <trading/trade.hpp>
#include <trading/fraction.hpp>
#include <trading/price_point.hpp>
#include <trading/currency.hpp>
#include <trading/close_position.hpp>

namespace trading {
    template<currency::crypto base, currency::crypto quote, typename Strategy, template<currency::crypto, currency::crypto> typename TradeManager, typename Stats>
    class test_box {
    private:
        std::optional<action> action_;
        std::vector<price_point<quote>> price_points_;
        TradeManager<base, quote> manager_;

    public:
        explicit test_box(std::vector<price_point<quote>> price_points, const TradeManager<base, quote>& manager)
            :price_points_ (std::move(price_points)), manager_(manager) { }

        template<typename ...Args>
        void operator()(Args... args)
        {
            std::vector<trade<base, quote>> closed;
            std::optional<trade<base, quote>> active;
            Strategy strategy;

            // create strategy
            try {
                strategy = Strategy(args...);
            }
            catch (...) {
                std::throw_with_nested(std::runtime_error("Cannot create strategy"));
            }

            // collect trades
            for (const auto& point : price_points_) {
                action_ = strategy(point.price());

                if (action_) {
                    manager_.update_active_trade(active, *action_, point);

                    // add to closed trades
                    if (active->is_closed()) {
                        closed.emplace_back(*active);
                        active = std::nullopt;
                    }
                }
            }

            // close active trade
            if (active) {
                const auto& point = price_points_.back();
                active->add_closed(close_position<base, quote>{active->size(), point.price(), point.time()});
                closed.emplace_back(*active);
            }

            // create stats
            Stats stats(closed);
            std::cout << "min profit: " << stats.min_profit()
                      << " %, max profit: " << stats.max_profit()
                      << " %, n closed trades: " << stats.n_closed_trades() << std::endl;
        }
    };
}

#endif //EMASTRATEGY_TEST_BOX_HPP
