// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/integer.hpp>
#include <data/encoding/hex.hpp>
#include <data/math/number/gmp/gmp.hpp>
#include <data/math/number/bytes/N.hpp>
#include <data/math/number/bytes/Z.hpp>
#include <data/io/unimplemented.hpp>
#include <algorithm>

namespace data::encoding {
    
    namespace decimal {
        
        bytes read(string_view s, endian::order r) {
            if (!valid(s)) throw std::invalid_argument{"not decimal"};
            math::number::N_bytes<endian::big> n{math::number::N{s}};
            if (r == endian::little) std::reverse(n.Value.begin(), n.Value.end());
            return n.Value;
        }
        
    };
    
    namespace hexidecimal {
        
        void write(std::ostream& o, bytes_view b, endian::order r) {
            o << "0x";
            if (r == endian::order::big) for (int i = 0; i < b.size(); i++) o << hex::write(b[i]);
            else for(int i = b.size() - 1; i >= 0; i--) o << hex::write(b[i]);
        }
        
        bytes read(string_view s, endian::order r) {
            if (!valid(s)) throw std::invalid_argument{"not hexidecimal"};
            bytes b = bytes(hex::string{s.substr(2)});
            if (r == endian::big) return b;
            std::reverse(b.begin(), b.end());
            return b;
        }
        
    };
    
    namespace natural {
        
        bytes read(string_view s, endian::order r) {
            if (!valid(s)) throw std::invalid_argument{"not a natural number"};
            if (hexidecimal::valid(s)) return hexidecimal::read(s, r);
            return decimal::read(s, r);
        }
        
    };
    
    namespace integer {
        
        bytes read(string_view s, endian::order r) {
            if (!valid(s)) throw std::invalid_argument{"not an integer"};
            if (hexidecimal::valid(s)) return hexidecimal::read(s, r);
            if (negative(s)) {
                math::number::Z_bytes<endian::big> n{math::number::Z{s}};
                if (r == endian::little) std::reverse(n.Value.begin(), n.Value.end());
                return n.Value;
            }
            return decimal::read(s, r);
        }
        
    };
    
}


