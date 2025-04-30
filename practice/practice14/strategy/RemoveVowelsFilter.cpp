#include "RemoveVowelsFilter.h"
#include "TextFilterStrategy.h"
#include <algorithm> 
#include <cctype> 
#include <string>

bool RemoveVowelsFilter::isVowel(char c) {
    c = std::tolower(c); 
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'); 
}

std::string RemoveVowelsFilter::filter(const std::string& text) {
    std::string result = "";
    for (char c : text) {
        if (!isVowel(c)) {
            result += c; 
        }
    }
    return result; 
}