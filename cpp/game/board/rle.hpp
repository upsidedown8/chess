#pragma once
#include <string>
#include <sstream>

class RLE {
public:
    static std::string encode(const std::string& text);
    static std::string decode(const std::string& text);
};
