#ifndef TEXTFILTERSTRATEGY_H
#define TEXTFILTERSTRATEGY_H

#include <string>
#include <memory>

class TextFilterStrategy {
public:
    virtual ~TextFilterStrategy() = default;
    virtual std::string filter(const std::string& text) = 0; // Ensure this matches the derived class
};

#endif // TEXTFILTERSTRATEGY_H