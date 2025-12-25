#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader
{
    public:
        // Construtor. It does not have parameters.
        CSVReader();

        // Static doesn't need objects. It belongs to the class, not an instance.
        static std::vector<OrderBookEntry> readCSV(std::string csvFile);
        static std::vector<std::string> tokenise(std::string csvLine, char separator);
        static OrderBookEntry stringsToOBE( std::string price, 
                                            std::string amount,
                                            std::string timestamp,
                                            std::string product,
                                            OrderBookType OrderBookType);

    private:
        static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
};