#include <string>
#include <map>
#include "OrderBook.h"
#include "OrderBookEntry.h"

#ifndef WALLET_H
#define WALLET_H

/** * The Wallet class manages the user's financial assets. 
 * It provides methods for tracking balances, verifying liquidity for trades, 
 * and updating holdings following successful market matches.
 */
class Wallet {
    public:
        /** Default constructor to initialise an empty wallet. */
        Wallet();

        /** Deposits a specified quantity of a particular currency into the wallet. */
        void insertCurrency(std::string type, double amount);

        /** * Attempts to withdraw a specified quantity of currency. 
         * Returns true if the operation is successful, false otherwise. 
         */
        bool removeCurrency(std::string type, double amount);

        /** * Verifies whether the wallet contains a balance equal to or 
         * greater than the requested amount for a specific currency. 
         */
        bool constainsCurrency(std::string type, double amount);

        /** * Generates a human-readable string representation of all 
         * current currency holdings within the wallet. 
         */
        std::string toString() const;

        /** * Evaluates if the wallet possesses sufficient funds to 
         * support the commitment of a specific ask or bid order. 
         */
        bool canFulfillOrder(OrderBookEntry order);

        /** * Adjusts wallet balances post-trade. This assumes the transaction 
         * was successfully executed and involves the wallet owner. 
         */
        void processSale (OrderBookEntry& sale);

        /** * Returns the current balance for a specific currency type. 
         * Returns 0 if the currency identifier is not found in the records.
         */
        double getCurrencyBalance(std::string type) {
            if (currencies.count(type) == 0) return 0;
            return currencies[type];
        }

    private:
        /** * Internal data structure mapping currency identifiers (e.g., "BTC") 
         * to their respective numerical balances. 
         */
        std::map<std::string, double> currencies;
};

#endif