#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include <map>

/** Enum representing the specific digital assets supported by the platform. */
enum class CryptoType { ETH, BTC, USDT, DOGE, UNKNOWN };

class MerkelMain
{
    public:

        /** * The class constructor. It is currently empty as the initialisation 
         * logic resides within the implementation file. 
         */
        MerkelMain();

        /** Initiates the primary execution loop of the exchange simulation. */
        void init();

    private: 
        /** Displays the primary command interface to the user. */
        void printMenu();

        /** Provides instructional guidance regarding the system's objectives. */
        void printHelp();

        /** Calculates and displays current market trends and extrema. */
        void printMarketStats();

        /** Processes the user's request to place a sell order (ask). */
        void enterAsk();

        /** Processes the user's request to place a buy order (bid). */
        void enterBid();

        /** Outputs the current asset balances of the authenticated user. */
        void printWallet();

        /** Transitions the market state to the next temporal interval. */
        void gotoNextTimeframe();

        /** * Captures and validates numerical input from the console, 
         * distinguishing between standard navigation and registration. 
         */
        int  getUserOption(bool isRegistration);

        /** Routes the user's selection to the corresponding system logic. */
        void processUserOption(int userOption);

        /** Displays OHLC (Open, High, Low, Close) data for specific products. */
        void printCandlestickStats();

        /** Authenticates a user against the persistent database records. */
        void login();

        /** Manages the creation and storage of a new user profile. */
        void registerUser();

        /** Retrieves registered user credentials from the persistent CSV storage. */
        void loadUsers();

        /** Facilitates the addition of funds to the active user's wallet. */
        void depositMoney();

        /** Facilitates the removal of funds from the active user's wallet. */
        void withdrawMoney();

        /** Synchronises the internal wallet state with the persistent storage. */
        void updateCSVBalance();

        /** Lists the most recent transactions executed by the current user. */
        void printTradeHistory();

        /** Aggregates quantitative data regarding user trading performance. */
        void printTradingStats();

        /** Runs a mock trading scenario to stress-test system persistence. */
        void simulateTrading();

        /** Reconstructs the user's wallet state from historical activity logs. */
        void loadUserWallet();

        /** Converts a textual asset name into its structured internal type. */
        CryptoType stringToCrypto(std::string s);

        /* * Member variables are declared here without parentheses as they represent 
         * the object's state. We specify them in the header so the compiler can 
         * accurately determine the memory allocation required for each instance. 
         * Temporary variables within methods are managed on the stack and do not 
         * belong in the class declaration. 
         */

        /** An associative container mapping user identifiers to their hashed credentials. */
        std::map<std::string, std::string> userDatabase;

        /** A secondary mapping to link unique user IDs with their display names. */
        std::map<std::string, std::string> idToName;

        /** Stores the ID of the authenticated user; an empty string indicates no active session. */
        std::string currentUser = "";

        /** Tracks the current simulated timeframe of the exchange. */
        std::string currentTime; 

        /** The primary order book instance, initialised with the source data file. */
        OrderBook orderBook{"20200601.csv"};

        /** An associative container linking user IDs to their respective financial wallets. */
        std::map<std::string, Wallet> userWallets;
};