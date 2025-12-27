#include "Wallet.h"
#include <ostream>
#include <iostream>
#include "CSVReader.h"

/** Default constructor for the Wallet class. */
Wallet::Wallet() {
    
};

/** Adds a specified amount of currency to the wallet balance. 
 * Throws an exception if the provided amount is negative. 
 */
void Wallet::insertCurrency(std::string type, double amount) {

    double balance;
    if (amount < 0) {
        throw std::exception{};
    }

    // Check if the currency already exists within the map.
    if(currencies.count(type) == 0) {
        balance = 0; // Initialise balance if type is not found.
    } 
    else {
        balance = currencies[type];
    }

    balance += amount;
    currencies[type] = balance;
};

/** Checks if the wallet contains at least the specified amount of a given currency. */
bool Wallet::constainsCurrency(std::string type, double amount) {
    if(currencies.count(type) == 0) {
        return false;
    }
    else {
        return currencies[type] >= amount;
    }
};

/** Deducts a specified amount from the wallet. 
 * Returns false if the amount is negative, the currency is missing, or funds are insufficient. 
 */
bool Wallet::removeCurrency(std::string type, double amount) {

    if (amount < 0) {
        return false;
    }

    if (currencies.count(type) == 0) {
        return false;
    }

    else {
        if (constainsCurrency(type, amount)) { // Verify sufficient liquidity.
            currencies[type] -= amount;
            return true;
        }
        else {
            return false; // Currency exists but balance is inadequate.
        }
    }
}

/** Returns a string representation of the current wallet holdings. */
std::string Wallet::toString() const {
    std::string s;
    for(std::pair<std::string, double> pair : currencies) {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    return s;
};

/** Determines if the wallet has enough funds to cover a potential bid or ask order. */
bool Wallet::canFulfillOrder(OrderBookEntry order) {
    // Split the product string (e.g., "ETH/BTC") into base and quote currencies.
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');

    // Scenario: User is selling (Ask). Must possess the base currency (e.g., ETH).
    if (order.orderType == OrderBookType::ask) {
        double amount = order.amount;
        std::string currency = currs[0];

        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;

        return constainsCurrency(currency, amount);
    };

    // Scenario: User is buying (Bid). Must possess the quote currency (e.g., BTC).
    if (order.orderType == OrderBookType::bid) {
        // Required balance is calculated as volume multiplied by price.
        double amount = order.amount * order.price;
        std::string currency = currs[1];

        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;

        return constainsCurrency(currency, amount);
    }

    return false;
};

/** Updates the wallet balances following a successful trade execution (match). */
void Wallet::processSale(OrderBookEntry& sale) {
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');

    // Case: The user's 'Ask' order was matched (User sold an asset).
    if (sale.orderType == OrderBookType::asksale) {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];

        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    };

    // Case: The user's 'Bid' order was matched (User bought an asset).
    if (sale.orderType == OrderBookType::bidsale) {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];

        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    };
};