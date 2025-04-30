#include <iostream>
#include <string>
#include <limits>
#include <memory>

#include "TextFilterStrategy.h"
#include "ReverseFilter.h"
#include "UppercaseFilter.h"
#include "RemoveVowelsFilter.h"
#include "CensorBadWordsFilter.h"


class TextProcessor {
private:
    std::unique_ptr<TextFilterStrategy> strategy;
public:
    void setStrategy(std::unique_ptr<TextFilterStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }
     std::string processText(const std::string& text) {
        if (strategy) {
            return strategy->filter(text);
        }
        return text; // No filtering applied
    }
};


int main() {
    std::string inputSentence;
    std::cout << "Enter a sentence: ";
    std::getline(std::cin, inputSentence);

    TextProcessor processor;
    int choice;

    do {
        std::cout << "\nChoose a filter strategy:\n";
        std::cout << "1. Reverse\n";
        std::cout << "2. Uppercase\n";
        std::cout << "3. Remove Vowels\n";
        std::cout << "4. Censor Bad Words\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        std::unique_ptr<TextFilterStrategy> currentStrategy;
        switch (choice) {
            case 1:
                currentStrategy = std::make_unique<ReverseFilter>();
                break;
            case 2:
                currentStrategy = std::make_unique<UppercaseFilter>();
                break;
            case 3:
                currentStrategy = std::make_unique<RemoveVowelsFilter>();
                break;
            case 4:
                currentStrategy = std::make_unique<CensorBadWordsFilter>();
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                continue;   
        }

        if (currentStrategy) {
            processor.setStrategy(std::move(currentStrategy));
            std::string processedText = processor.processText(inputSentence);
            std::cout << "Processed text: " << processedText << std::endl;
        }

    } while (choice != 0);

    return 0;
    }
