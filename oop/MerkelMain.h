#include <vector>
#include "OrderBookEntry.h"


// Aqui como é ".h", pensa em head, que fica above body. Entao a gente coloca a classe aqui.

class MerkelMain {
    public:
        // Publicos precisamos ter apenas o init e o construtor.
        MerkelMain();
        void init();

    private:
        void loadOrderBook();
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterOffer();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        void processUserOption(int userOption);
        int  getUserOption();

        // Criar vetor para armazenar instancias de uma classe nova.
        std::vector<OrderBookEntry> orders;
};