/** Ensure this header file is included only once during the compilation process. */
#pragma once
#include <string>

/** * Defines a custom data type 'OrderBookType' using a scoped enum.
 * It restricts values to a predefined set: bid, ask, unknown, asksale, and bidsale.
 * Note: An 'enum class' provides named constants without allocating memory for 
 * methods or attributes.
 */
enum class OrderBookType{bid, ask, unknown, asksale, bidsale};

class OrderBookEntry
{
    public:

        /** * Constructor Declaration: To instantiate an OrderBookEntry, five primary 
         * values must be provided as parameters.
         * The underscore (e.g., _price) is a naming convention to distinguish 
         * parameters from internal member variables.
         * @param username Defaults to "dataset" if no specific identifier is provided.
         */
        OrderBookEntry( double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType,
                        std::string username="dataset"); 

        /** * A static utility function that converts a string literal into an 
         * OrderBookType enum. Being 'static' means it can be invoked without 
         * the need to instantiate a specific object.
         */
        static OrderBookType stringToOrderBookType(std::string s); 

        /* Internal member variables (attributes): These represent the permanent 
           memory allocated to each class instance. In a large dataset, each 
           row effectively becomes a unique object in memory. */
        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
        std::string username;

        /** Static comparator to sort entries chronologically by timestamp. */
        static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2) {
            return e1.timestamp < e2.timestamp;
        };

        /** Static comparator to sort entries by price in ascending order. */
        static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2) {
            return e1.price < e2.price;
        };

        /** Static comparator to sort entries by price in descending order. */
        static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2) {
            return e1.price > e2.price;
        };
      
};


/*
---------------------------------------------
COMPREHENSIVE CLASS STRUCTURE SUMMARY
---------------------------------------------

class ClassName {
public:
    // 1. Constructor (Matches class name; optional for object initialisation)
    ClassName(type1 param1, type2 param2);

    // 2. Member Functions (Methods)
    return_type function1();
    void function2(type x);

    // 3. Member Variables (Attributes)
    type1 attribute1;
    type2 attribute2;
};

---------------------------------------------
*/