#include "UppercaseFilter.h"
#include "TextFilterStrategy.h"
#include <algorithm> // For std::transform
#include <cctype> // For std::toupper
#include <string>

std::string UppercaseFilter::filter(const std::string& text) {
    std::string uppercase_text = text; 
    std::transform(uppercase_text.begin(), uppercase_text.end(), uppercase_text.begin(), ::toupper); 
    return uppercase_text; 
}