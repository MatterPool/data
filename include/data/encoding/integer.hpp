// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <data/types.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <data/encoding/endian.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <ctre.hpp>

namespace data::encoding {
    namespace decimal {
        constexpr auto pattern = ctll::fixed_string{"^0|([1-9][0-9]*)$"};
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool nonzero(string_view s) {
            return valid(s) && s[0] != '0';
        }
        
        inline uint32 digits(string_view s) {
            return valid(s) ? s.size() : 0;
        }
        
        bytes read(string_view s, endian::order r);
    };
    
    namespace hexidecimal {
        constexpr auto pattern = ctll::fixed_string{"0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        
        constexpr auto zero_pattern = ctll::fixed_string{"^0x(00)*$"};
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        inline uint32 digits(string_view s) {
            return valid(s) ? s.size() - 2 : 0;
        }
        
        bytes read(string_view s, endian::order r);
        
        void write(std::ostream& o, bytes_view b, endian::order r);
    };
    
    namespace natural {
        constexpr auto pattern = ctll::fixed_string{
            "^0|([1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))$"};
        
        constexpr auto zero_pattern = ctll::fixed_string{"^0|0x(00)*$"};
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        inline uint32 digits(string_view s) {
            return std::max(decimal::digits(s), hexidecimal::digits(s));
        }
        
        bytes read(string_view s, endian::order r);
    };
    
    namespace integer {
        constexpr auto pattern = ctll::fixed_string{
            "^0|(-?[1-9][0-9]*)|(0x((([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)))$"};
        
        constexpr auto zero_pattern = ctll::fixed_string{
            "^0|0x(00)*$"};
        
        constexpr auto negative_pattern = ctll::fixed_string{
            "^(-([1-9][0-9]*))|0x(([8-9a-f][0-9a-f]([0-9a-f][0-9a-f])*)|([8-9A-F][0-9A-F]([0-9A-F][0-9A-F])*))"};
        
        inline bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        inline bool negative(string_view s) {
            return ctre::match<negative_pattern>(s);
        }
        
        inline bool zero(string_view s) {
            return ctre::match<zero_pattern>(s);
        }
        
        inline bool nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        inline uint32 digits(string_view s) {
            return negative(s) ? natural::digits(s.substr(1, s.size() - 1)) : natural::digits(s);
        }
        
        bytes read(string_view s, endian::order r);
        
    };
    
}

#endif

