#ifndef DATA_MATH_MODULE
#define DATA_MATH_MODULE

#include "ring.hpp"

namespace data {
    
    namespace math {
            
        template <typename M, typename R>
        struct module {
            static const group::abelian<M> r1;
            static const ring<R> r2;
                
            M times(M m, R r) const {
                return m * r;
            }
        };
    
    }
    
}

#endif 
