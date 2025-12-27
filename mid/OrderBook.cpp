#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "Candlestick.h"

/*
NOTE: 
    - Observe that we have implemented all methods declared in OrderBook.h here. 
      The signatures are identical; we have simply provided the function bodies {}.
    - We must prefix each method name with the class scope resolution operator (OrderBook::).
*/

/** Constructor: Initialises the order book by reading a CSV data file. */
OrderBook::OrderBook(std::string filename) {
    orders = CSVReader::readCSV(filename);
};

/** Returns a vector containing all unique product names identified in the dataset. */
std::vector<std::string> OrderBook::getKnownProducts() {
    std::vector<std::string> products;

    /* A map utilizes key-value pairs. Each element is an std::pair<const Key, T>.
       We use it here to ensure product names are unique, as maps do not allow duplicate keys.
       The map class is a standard component of the C++ Standard Library. */
    std::map<std::string, bool> prodMap;

    for(OrderBookEntry& e : orders) {
        prodMap[e.product] = true;
    }

    // Extract the unique keys (product names) into the return vector.
    for (auto const& e : prodMap) {
        products.push_back(e.first);
    }

    return products;
};

/** Returns a filtered subset of orders matching specific criteria: type, product, and timeframe. */
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                std::string product,
                                                std::string timestamp) 
{
    std::vector<OrderBookEntry> orders_sub;

    for(OrderBookEntry& e : orders) {
        if(e.orderType == type && e.product == product && e.timestamp == timestamp) {
            orders_sub.push_back(e);
        }
    }

    return orders_sub;
};

/** Identifies the maximum price within a provided set of order entries. */
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders) {
    double max = orders[0].price;

    for(OrderBookEntry& e : orders) {
        if (e.price > max) {
            max = e.price;
        }
    }
    return max;
};

/** Identifies the minimum price within a provided set of order entries. */
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders) {
    double min = orders[0].price;

    for(OrderBookEntry& e : orders) {
        if (e.price < min) {
            min = e.price;
        }
    }
    return min;
};

/** Retrieves the timestamp associated with the very first entry in the order book. */
std::string OrderBook::getEarliestTime() {
    return orders[0].timestamp;
};

/** Locates the next chronological timeframe following the provided timestamp. */
std::string OrderBook::getNextTime(std::string timestamp) {
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders) {
        if (e.timestamp > timestamp) {
            next_timestamp = e.timestamp;
            break;
        }
    }

    // If no subsequent timeframe is found, loop back to the beginning.
    if(next_timestamp == "") {
        next_timestamp = orders[0].timestamp;
    }
    
    return next_timestamp;
};

/** Inserts a new order into the book and re-sorts the collection chronologically. */
void OrderBook::insertOrder(OrderBookEntry & order) {
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
};

/** Executes the matching engine logic to pair compatible asks and bids for a specific product. */
std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp) {
    
    // Retrieve all active asks for the given timeframe.
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);

    // Retrieve all active bids for the given timeframe.
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);

    std::vector<OrderBookEntry> sales;

    // Sort asks in ascending order (lowest prices first).
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);

    // Sort bids in descending order (highest prices first).
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    // Output market depth statistics for debugging purposes.
    std::cout << "max ask " << asks[asks.size()-1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size()-1].price << std::endl;

    // Iterative matching process.
    for(OrderBookEntry& ask : asks) {
        for (OrderBookEntry& bid : bids) {
            // Check if the bid price meets or exceeds the ask price.
            if (bid.price >= ask.price) {

                // Initialise a sale entry; default to asksale.
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                // Attribute the sale to the simulated user if applicable.
                if (bid.username == "simuser") {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;;
                }

                if (ask.username == "simuser") {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;;
                }

                // Scenario A: Exact match in volume.
                if (bid.amount == ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }

                // Scenario B: Bid volume exceeds ask volume.
                if (bid.amount > ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break;
                }

                // Scenario C: Ask volume exceeds bid volume (Partial fill).
                if (bid.amount < ask.amount && bid.amount > 0) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue; // Proceed to the next bid to fulfill remaining ask volume.
                }
            }
        }
    }
    return sales;
};


/** * Computes Candlestick metrics (OHLC) from a vector of orders. 
 * Note the use of a reference (&) to avoid unnecessary memory duplication.
 */
std::vector<Candlestick> OrderBook::computeCandlesticks(std::vector<OrderBookEntry>& orders){
    std::vector<Candlestick> candlesticks;

    if (orders.size() == 0) return candlesticks;

    // Define Open and Close prices based on chronological boundaries.
    double open = orders[0].price;
    double close = orders[orders.size() - 1].price;

    // Initialise High and Low with the first available price.
    double high = orders[0].price;
    double low = orders[0].price;

    for (OrderBookEntry& e : orders) 
        {
            // Update the 'high' record if the current order price is greater.
            if (e.price > high) high = e.price;
            
            // Update the 'low' record if the current order price is lower.
            if (e.price < low) low = e.price;
        }

    Candlestick candle{orders[0].timestamp, open, high, low, close};
    candlesticks.push_back(candle);

    return candlesticks;
}