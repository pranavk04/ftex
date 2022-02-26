//
// Created by Pranav Konda on 2/25/22.
//

#ifndef FTEX_UTIL_H
#define FTEX_UTIL_H

#include <string>
#include <sstream>

namespace util {
    inline void replace_all(std::string& s, std::string const& dst, std::string const& rep) {
        size_t pos = 0;
        while((pos = s.find(dst, pos)) != std::string::npos) {
            s.replace(pos, dst.length(), rep);
            pos+=rep.length();
        }
    }
}
#endif // FTEX_UTIL_H
