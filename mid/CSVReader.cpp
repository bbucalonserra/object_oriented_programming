#include "CSVReader.h"
#include <iostream>
#include <fstream>

// Default constructor for CSVReader.
// No initialisation logic is required at this stage.
CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    // Vector used to store all parsed OrderBookEntry objects read from the CSV file.
    std::vector<OrderBookEntry> entries;

    // Input file stream used to open and read the CSV file from disk.
    std::ifstream csvFile{csvFilename};

    // Temporary string used to hold each line read from the file sequentially.
    std::string line;

    // Checks whether the file has been successfully opened before attempting to read.
    if (csvFile.is_open())
    {
        // Reads the file line by line using newline as the default delimiter.
        while(std::getline(csvFile, line))
        {
            try {
                // Converts the current CSV line into an OrderBookEntry object.
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                // Stores the successfully parsed entry in the vector.
                entries.push_back(obe);
            }catch(const std::exception& e)
            {
                // Invalid or malformed data lines are ignored silently.
                //std::cout << "CSVReader::readCSV bad data"  << std::endl;
            }
        } // end of while
    }    

    // Outputs the total number of valid entries read from the CSV file.
    std::cout << "CSVReader::readCSV read " << entries.size() << " entries"  << std::endl;
    return entries; 
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
   // Vector used to store individual tokens extracted from the CSV line.
   std::vector<std::string> tokens;

   // Indices used to track the beginning and end of each token.
   signed int start, end;

   // Temporary string used to store each extracted token.
   std::string token;

    // Finds the first character that is not a separator.
    start = csvLine.find_first_not_of(separator, 0);

    do{
        // Finds the next occurrence of the separator character.
        end = csvLine.find_first_of(separator, start);

        // Terminates parsing if no valid token boundaries are found.
        if (start == csvLine.length() || start == end) break;

        // Extracts the substring representing the token.
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);

        // Stores the extracted token.
        tokens.push_back(token);

        // Advances the start index to the character following the separator.
        start = end + 1;

    }while(end > 0);

   // Returns the vector containing all parsed tokens.
   return tokens; 
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    // Variables used to store numerical values converted from strings.
    double price, amount;

    // Valid CSV lines are expected to contain exactly five tokens.
    if (tokens.size() != 5)
    {
        //std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }

    // Attempts to convert price and amount fields to double precision values.
    try {
         price = std::stod(tokens[3]);
         amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        // Reports conversion errors related to invalid floating-point values.
        std::cout << "Bad float! " << tokens[3]<< std::endl;
        std::cout << "Bad float! " << tokens[4]<< std::endl; 
        throw;        
    }

    // Constructs an OrderBookEntry object using the parsed and converted data.
    OrderBookEntry obe{price, 
                        amount, 
                        tokens[0],
                        tokens[1], 
                        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe; 
}
      
OrderBookEntry CSVReader::stringsToOBE( std::string priceString, 
                                        std::string amountString,
                                        std::string timestamp,
                                        std::string product,
                                        OrderBookType orderType)
{
    // Variables used to store converted numerical values.
    double price, amount;

    try {
        // Converts price and amount from string representation to double.
        price = std::stod(priceString);
        amount = std::stod(amountString);
    } catch (const std::exception& e) {
        // Outputs diagnostic information if conversion fails.
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString<<std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString<<std::endl;
        throw;
    }

    // Constructs and returns a fully initialised OrderBookEntry object.
    OrderBookEntry obe{price, 
                        amount, 
                        timestamp,
                        product, 
                        orderType};

    return obe;
}
