#include "ReverseFilter.h"
#include "TextFilterStrategy.h"
#include <algorithm> // For std::reverse

std::string ReverseFilter::filter(const std::string& text) const {
    std::string reversedText = text; 
    std::reverse(reversedText.begin(), reversedText.end()); 
    return reversedText; 
}