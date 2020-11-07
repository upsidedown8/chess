#include "game/rle.hpp"

std::string RLE::encode(const std::string& text) {
    std::stringstream ss{};
    for (size_t textIndex = 0; textIndex < text.length(); textIndex++){
        int rl = 1;
        while (textIndex+1 < text.length() && text[textIndex] == text[textIndex+1]){
            rl++;
            textIndex++;
        }
        if (rl > 1)
            ss << rl;
        ss << text[textIndex];
    }
    return ss.str();
}
std::string RLE::decode(const std::string& text) {
    std::stringstream ss{};
    size_t textIndex = 0;
    while (textIndex < text.length()) {
        int digits = 0;
        while(isdigit(text[textIndex + digits]))
            digits++;
        int runLength = digits > 0 ? stoi(text.substr(textIndex, textIndex + digits)) : 1;
        for (int x = 0; x < runLength; x++)
            ss << text[textIndex + digits];
        textIndex += digits + 1;
    }
    return ss.str();
}