// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_DATA
#define DATA_DATA

#include <data/math/number/bounded/bounded.hpp>
#include <data/list/linked.hpp>
#include <data/map/rb.hpp>
#include <data/tools/map_set.hpp>
#include <data/tools/channel.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/tree/linked.hpp>
#include <data/valid.hpp>
#include <data/plus.hpp>
#include <data/tools/priority_queue.hpp>
#include <data/tools/ordered_list.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/rational.hpp>
#include <data/math/number/eratosthenes.hpp>
#include <data/math/polynomial.hpp>
#include <data/math/complex.hpp>
#include <data/crypto/ripemd160.hpp>
#include <data/crypto/sha256.hpp>
#include <data/crypto/sha512.hpp>
#include <data/fold.hpp>
#include <data/for_each.hpp>

// using namespace data::exported;
// to get all complete features of this library conveniently. 
namespace data::exported {
    
    // Natural numbers
    using N = math::number::N;
    
    // Integers
    using Z = math::number::Z;
    
    // Rationals. 
    using Q = math::number::fraction<Z, N>;
    
    // Gaussian numbers (complex rationals)
    using G = math::complex<Q>;
    
    // Primes
    using prime = math::number::prime<N>;
    
    // The sieve of Eratosthenes.
    using eratosthenes = math::number::eratosthenes<N>;
    
    // fixed-size numbers of any size. 
    template <size_t size> using uint = data::uint<size>;
    template <size_t size> using integer = data::integer<size>;
    
    // Polynomials
    template <typename X> 
    using polynomial = math::polynomial<X, N>;
    
    // a functional list. 
    template <typename X> using list = data::list::linked<X>;
    
    // functional queue built using the list. 
    template <typename X> using queue = data::functional_queue<list<X>>;
    
    // tree. 
    template <typename X> using tree = data::tree::linked<X>;
    
    // a functional map implemented as a red-black tree
    // wrapper of Milewski's implementation of Okasaki.
    template <typename K, typename V> using map = data::rb_map<K, V>;
    
    // priority queue. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using priority_queue = data::priority::queue<X>;
    
    // ordered_list. wrapper of Milewski's implementation of Okasaki.
    template <typename X> using ordered_list = data::ordered_list<X>;
    
    // get all values from a map with the given keys. 
    template <typename key, typename value, typename map>
    list<value> get_all(map m, list<key> k);
    
    // set implemented as a map. 
    template <typename X> using set = data::map_set<map<X, bool>>;
    
    // for_each applies a function to the members of a data structure and 
    // constructs another data structure of the same shape containing the outputs. 
    // see data/for_each.hpp
    
    // fold takes a function, an initial value, and a list and 
    // successively applies the function to the accumulated output
    // and the next element of the list. 
    // see data/fold.hpp
    
    // reduce is like fold except that it does not take an initial
    // value. 
    // see data/fold.hpp
    
    // Thread safe communication channel, similar to golang. 
    template <typename X> using chan = channel<X>;
    
}

#endif
