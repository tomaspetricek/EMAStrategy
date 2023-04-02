//
// Created by Tomáš Petříček on 30.03.2023.
//

#ifndef BACKTESTING_SYSTEMATIC_GENERATORS_HPP
#define BACKTESTING_SYSTEMATIC_GENERATORS_HPP

#include <trading/generators.hpp>

namespace trading::systematic {
        // generates unique subsequent fractions in interval (0, max), such as max is in interval (0.0, 1.0]
        template<std::size_t n_levels>
        class levels_generator : public trading::levels_generator<n_levels> {
        public:
            using value_type = cppcoro::recursive_generator<std::array<fraction_t, n_levels>>;

            explicit levels_generator(size_t n_unique = n_levels)
                    :trading::levels_generator<n_levels>(n_unique) { }

            value_type operator()()
            {
                co_yield generate<0>(this->denom_);
            }

        private:
            template<std::size_t depth = n_levels>
            requires (depth==n_levels)
            value_type generate(std::size_t) { co_yield this->levels_; }

            template<std::size_t depth = 0>
            requires (depth<n_levels)
            value_type generate(std::size_t prev_num)
            {
                for (std::size_t num{--prev_num}; num>n_levels-depth-1; num--) {
                    this->levels_[depth] = fraction_t{num, this->denom_};
                    co_yield generate<depth+1>(num);
                }
            }
        };

        // generates fractions that add up to 1.0
        template<std::size_t n_sizes>
        class sizes_generator : public trading::sizes_generator<n_sizes> {
        public:
            using value_type = cppcoro::recursive_generator<std::array<fraction_t, n_sizes>>;

            explicit sizes_generator(size_t n_unique)
                    :trading::sizes_generator<n_sizes>(n_unique) { }

            value_type operator()()
            {
                co_yield generate<0>(this->denom_);
            }

        private:
            template<std::size_t depth = n_sizes>
            requires (depth+1==n_sizes)
            value_type generate(std::size_t remaining)
            {
                this->sizes_[depth] = fraction_t{remaining, this->denom_};
                co_yield this->sizes_;
            }

            template<std::size_t depth = 0>
            requires (depth+1<n_sizes)
            value_type generate(std::size_t remaining)
            {
                std::size_t max = (remaining>this->max_num_) ? this->max_num_ : remaining-1;
                for (std::size_t num{1}; num<=max; num++) {
                    this->sizes_[depth] = fraction_t{num, this->denom_};
                    co_yield generate<depth+1>(remaining-num);
                }
            }
        };

        class int_range : public trading::int_range {
        public:
            using value_type = cppcoro::generator<int>;

            int_range(int from, int to, int step)
                    :trading::int_range(from, to, step) { }

            value_type operator()()
            {
                for (int val{this->from_}; val!=this->to_+this->step_; val += this->step_)
                    co_yield val;
            }
        };
    }

#endif //BACKTESTING_SYSTEMATIC_GENERATORS_HPP
