#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include <map>

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
        int  getUserOption(bool isRegistration);
        void processUserOption(int userOption);
        void printCandlestickStats();
        void login();
        void registerUser();
        void loadUsers();


        // Nao tem "()" porque é uma variável.
        // Precisamos escrever as variáveis no header (.h) porque o compilador precisa saber quanta memória reservar para o objeto.
        // As dentro do método são temporárias, entao não precisam estar aqui.

        // Um mapa ("dicionario chave valor") que possui duas strings.
        std::map<std::string, std::string> userDatabase;

        //
        std::map<std::string, std::string> idToName;

        // Estado. Se estiver vazia, não há sessão ativa.
        std::string currentUser = "";

        // Current time variable.
        std::string currentTime; 

        // Orderbook with CSV.
        OrderBook orderBook{"20200601.csv"};

        // Map (dictionary) of userId and wallets.
        std::map<std::string, Wallet> userWallets;
};