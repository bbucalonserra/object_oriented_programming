#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"


class MerkelMain
{
    public:

        // Função construtora. Ela esta vazia porque se olhar em merkelmain.cpp, o MerkelMain:MerkelMain está vazio
        MerkelMain();

        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime; // Nao tem "()" porque é uma variável.

        OrderBook orderBook{"20200317.csv"};

}; 
