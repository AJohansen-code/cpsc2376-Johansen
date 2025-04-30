#ifndef UPPERCASEFILTER_H
#define UPPERCASEFILTER_H

#include "TextFilterStrategy.h"
#include <string>

class UppercaseFilter : public TextFilterStrategy {
public:
    std::string filter(const std::string& text) override; 
};

#endif // UPPERCASEFILTER_H