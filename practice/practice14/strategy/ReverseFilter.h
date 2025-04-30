#ifndef REVERSEFILTER_H
#define REVERSEFILTER_H

#include "TextFilterStrategy.h"
#include <string>

class ReverseFilter : public TextFilterStrategy {
public:
    std::string filter(const std::string& text) override;
};

#endif // REVERSEFILTER_H