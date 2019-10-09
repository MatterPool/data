// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>

namespace  data::encoding::hex {
    
    string::string(const std::string& sourceString) : std::string(sourceString) {
        try {
            boost::algorithm::unhex(std::string::begin(), std::string::end(), std::back_inserter(Bytes));
        }
        catch(boost::algorithm::hex_decode_error exception)
        {
            throw invalid(format, *this);
        }
        ToBytes=&Bytes;
    }
    
    bool valid(const std::string &sourceString) {
        
        if(sourceString.size() %2 ==1)
            return false;
        std::string upper=boost::to_upper_copy(sourceString);
        for(int i=0;i<upper.size();i++)
        {
            if(upper[i]<48  || upper[i] > 70)
                return false;
            if(upper[i] > 57 && upper[i] < 65)
                return false;
        }
        return true;
    }
    
    std::string write(const bytes_view sourceBytes) {
        std::string output;
        boost::algorithm::hex(sourceBytes.begin(),sourceBytes.end(),std::back_inserter(output));
        return data::string(output);
    }
}
