//
// Created by Tomáš Petříček on 21.08.2022.
//

#ifndef BACKTESTING_DATA_POINT_HPP
#define BACKTESTING_DATA_POINT_HPP

#include <boost/date_time/posix_time/ptime.hpp>
#include <trading/types.hpp>

namespace trading {
    template<class Data>
    struct data_point {
        std::time_t time;
        Data data;

        friend bool operator==(const data_point& lhs, const data_point& rhs)
        {
            return lhs.time==rhs.time &&
                    lhs.data==rhs.data;
        }
    };

    using price_point = data_point<price_t>;
}

#endif //BACKTESTING_DATA_POINT_HPP
