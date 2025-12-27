#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include "Candlestick.h"

class OrderBook {
    public:
        /** Constructor: Initialises the system by reading from a specified CSV data file. */
        OrderBook(std::string filename);

        /** Returns a vector containing all unique product identifiers (strings) found in the book. */
        std::vector<std::string> getKnownProducts();

        /** * Retrieves a subset of orders based on type, product name, and timeframe.
         * Returns a vector of OrderBookEntry objects, representing the filtered data.
         */
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                            std::string product,
                                            std::string timestamp
                                    );

        /** * Calculates the maximum price within the provided collection of orders.
         * The '&' symbol indicates the vector is passed by reference, meaning:
         * - The function operates directly on the original vector rather than creating a copy.
         * - This is significantly more memory-efficient for large datasets (e.g., millions of rows).
         */
        static double getHighPrice(std::vector<OrderBookEntry>& orders);

        /** Calculates the minimum price within the provided collection of orders. */
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        /** Identifies the earliest timestamp available in the order book. */
        std::string getEarliestTime();

        /** Returns the subsequent chronological timestamp relative to the provided input. */
        std::string getNextTime(std::string timestamp);

        /** Inserts a specific order entry into the existing collection. */
        void insertOrder(OrderBookEntry & order);

        /** Matches compatible buy (bid) and sell (ask) orders to simulate trade execution. */
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        /** * Generates OHLC (Candlestick) data from a series of order entries.
         * Declared as 'static' as it performs a pure calculation that does not 
         * require an instance of the class to function.
         */
        static std::vector<Candlestick> computeCandlesticks(std::vector<OrderBookEntry>& orders);

    private:
        /** * Internal storage for all order entries. This is declared here so that each 
         * instance of the OrderBook class maintains its own unique collection of data.
         */
        std::vector<OrderBookEntry> orders;
        
};