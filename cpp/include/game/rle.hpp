#pragma once
#include <string>
#include <sstream>

namespace RLE {
    std::string encode(const std::string& text);
    std::string decode(const std::string& text);
};
