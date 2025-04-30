#include "CensorBadWordsFilter.h"
#include "TextFilterStrategy.h"
#include <algorithm> 
#include <cctype> 
#include <string>
#include <sstream> 

CensorBadWordsFilter::CensorBadWordsFilter() : badWords({"damn", "hell", "shit", "crap", "asshole"}) {}

std::string CensorBadWordsFilter::censorWord(const std::string& word) {
    return std::string(word.length(), '*'); // Replace the word with asterisks
}

std::string CensorBadWordsFilter::filter(const std::string &text) {
    std::string result = ""; 
    std::stringstream ss(text);
    std::string word;

    while (ss >> word) { 
        std::string lowerWord = word;
        std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower); 
        bool isBad = false;
        for (const auto& badWord : badWords) {
            if (lowerWord == badWord) {
                result += censorWord(word) + " "; // Censor the word
                isBad = true; 
                break; 
            }
        }
        if (!isBad) {
            result += word + " "; // Add the word as is
        }
    }
    if (!result.empty()) {
        result.pop_back(); // Remove the trailing space
    }   
    return result; 
}