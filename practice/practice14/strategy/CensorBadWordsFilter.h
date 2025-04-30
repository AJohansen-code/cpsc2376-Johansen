#ifndef CENSORBADWORDSFILTER_H
#define CENSORBADWORDSFILTER_H

#include "TextFilterStrategy.h"
#include <string>
#include <vector>

class CensorBadWordsFilter : public TextFilterStrategy {
private:
    std::vector<std::string> badWords;
    std::string censorWord(const std::string& word);
public:
    CensorBadWordsFilter();
    std::string filter(const std::string& text) override;
};

#endif // CENSORBADWORDSFILTER_H