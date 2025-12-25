#include <string>
#include <map>
#include "OrderBook.h"
#include "OrderBookEntry.h"
#ifndef WALLET_H
#define WALLET_H

class Wallet {
    public:
        Wallet();

        /**Insert currency to the wallet.*/
        void insertCurrency(std::string type, double amount);

        /** */
        bool removeCurrency(std::string type, double amount);

        /**Check if the wallet contains this much currency or more.*/
        bool constainsCurrency(std::string type, double amount);

        /**Generate string representation of the wallet. */
        std::string toString();

        /**Checks if the wallet can cope with this ask or bid.*/
        bool canFulfillOrder(OrderBookEntry order);

        /**Update contents of the wallet. Assumes the order was made by the owner of the wallet.*/
        void processSale (OrderBookEntry& sale);

    private:
        std::map<std::string, double> currencies;
};
#endif