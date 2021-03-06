// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_ABS
#define DATA_MATH_NUMBER_ABS

#include <data/types.hpp>
#include <data/io/unimplemented.hpp>

namespace data::math::number {

    template <typename N, typename Z> 
    struct abs {
        N operator()(const Z i) {
            return i < 0 ? -i : i;
        }
    };
}

#endif
