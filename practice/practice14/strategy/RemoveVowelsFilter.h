#ifndef REMOVEVOWELSFILTER_H
#define REMOVEVOWELSFILTER_H

#include "TextFilterStrategy.h"
#include <string>

class RemoveVowelsFilter : public TextFilterStrategy {
private:
    bool isVowel(char c);
public:
    std::string filter(const std::string& text) override; 
};

#endif // REMOVEVOWELSFILTER_H