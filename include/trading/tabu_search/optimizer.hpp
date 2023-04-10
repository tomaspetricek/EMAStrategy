//
// Created by Tomáš Petříček on 20.02.2023.
//

#ifndef BACKTESTING_TABU_SEARCH_OPTIMIZER_HPP
#define BACKTESTING_TABU_SEARCH_OPTIMIZER_HPP

#include <unordered_map>
#include <trading/concepts.hpp>
#include <trading/optimizer.hpp>

namespace trading::tabu_search {
    template<class ConcreteTabuTenure>
    concept TabuTenure = std::invocable<ConcreteTabuTenure> &&
            std::same_as<std::size_t, std::invoke_result_t<ConcreteTabuTenure>>;

    template<class ConcreteNeighborhoodSizer, class Optimizer>
    concept NeighborhoodSizer = std::invocable<ConcreteNeighborhoodSizer, const Optimizer&> &&
            std::same_as<std::size_t, std::invoke_result_t<ConcreteNeighborhoodSizer, const Optimizer&>>;

    template<class ConcreteNeighbor, class Config, class Movement>
    concept Neighbor = std::invocable<ConcreteNeighbor, const Config&> &&
            std::same_as<std::tuple<Config, Movement>, std::invoke_result_t<ConcreteNeighbor, const Config&>>;

    template<class ConcreteAspirationCriteria, class State, class Optimizer>
    concept AspirationCriteria = std::invocable<ConcreteAspirationCriteria, const State&, const Optimizer&> &&
            std::same_as<bool, std::invoke_result_t<ConcreteAspirationCriteria, const State&, const Optimizer&>>;

    template<class Config>
    class optimizer {
        state<Config> best_, curr_;
        std::size_t it_{0};

    public:
        using state_type = state<Config>;

        template<class Result, class TabuList, class... Observer>
        void operator()(const Config& init, Result& result,
                const Constraints<state_type> auto& constraints,
                ObjectiveFunction<Config> auto&& objective,
                TabuList tabu_list,
                Neighbor<Config, typename TabuList::move_type> auto&& neighbor,
                NeighborhoodSizer<optimizer> auto&& neighborhood_size,
                TerminationCriteria<optimizer> auto&& terminate,
                AspirationCriteria<state_type, optimizer> auto&& aspire,
                Observer& ... observers)
        {
            best_ = curr_ = state_type{init, objective(init)};
            state_type candidate, origin;
            typename TabuList::move_type curr_move, candidate_move;
            std::size_t neighborhood;

            (observers.started(*this), ...);
            for (; !terminate(*this); it_++) {
                // explore neighborhood
                origin = curr_;
                std::tie(curr_.config, curr_move) = neighbor(origin.config);
                curr_.value = objective(curr_.config);

                neighborhood = neighborhood_size(*this);
                assert(neighborhood>=1);
                for (std::size_t i{0}; i<neighborhood-1; i++) {
                    std::tie(candidate.config, candidate_move) = neighbor(origin.config);

                    if (!tabu_list.contains(candidate_move)) {
                        candidate.value = objective(candidate.config);

                        if (result.compare(candidate, curr_) || aspire(candidate, *this)) {
                            curr_ = candidate;
                            curr_move = candidate_move;
                        }
                    }
                }

                // try update best state
                if (result.compare(curr_, best_)) {
                    best_ = curr_;
                    if (constraints(best_))
                        result.update(best_);
                }

                tabu_list.forget();
                tabu_list.remember(curr_move);
                (observers.iteration_passed(*this, tabu_list), ...);
            }
            (observers.finished(*this), ...);
        }

        const state_type& best_state() const
        {
            return best_;
        }

        const state_type& current_state() const
        {
            return curr_;
        }

        std::size_t it() const
        {
            return it_;
        }
    };
}

#endif //BACKTESTING_TABU_SEARCH_OPTIMIZER_HPP