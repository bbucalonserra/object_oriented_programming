#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"

MerkelMain::MerkelMain() {

};

// Pra começar a rodar o construtor.
void MerkelMain::init() {

    // Aqui eu usei uma função dentro de init que eu criei só no final deste arquivo. Isto só é possivel porque
    // eu já a coloquei no header (.h).
    loadOrderBook();

    int input;
    while(true) {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
};

void MerkelMain::printMenu() {
    // 1 print help
    std::cout << "1: Print help " << std::endl;

    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;

    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;

    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;

    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;

    // 6 continue
    std::cout << "6: Continue " << std::endl;

    std::cout << "============ " << std::endl;
}

void MerkelMain::printHelp() {
    std::cout << "Help - your aim is to make money. Analyze the market and maek a bids and offers." << std::endl;
}

void MerkelMain::printMarketStats() {
    std::cout << "OrderBook contains: " << orders.size() << "entries" << std::endl;
}

void MerkelMain::enterOffer() {
    std::cout << "Make an offer - enter the amount." << std::endl;
}

void MerkelMain::enterBid() {
    std::cout << "Make a bid - enter the amount." << std::endl;
}

void MerkelMain::printWallet() {
    std::cout << "Your wallet is empty." << std::endl;
}

void MerkelMain::gotoNextTimeframe() {
    std::cout << "Going to next time frame." << std::endl;
}

int MerkelMain::getUserOption() {
    int userOption;

    std::cout << "Type in 1-6" << std::endl;
    std::cin >> userOption;
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption) {

    if (userOption == 0) {
    std::cout << "Invalid Choice." << std::endl;
    }

    if (userOption == 1) {
        printHelp();
    }

    if (userOption == 2) {
        printMarketStats();
    }

    if (userOption == 3) {
        enterOffer();
    }
    
    if (userOption == 4) {
        enterBid();
    }

    if (userOption == 5) {
        printWallet();
    }
    
    if (userOption == 6) {
        gotoNextTimeframe();
    }
}

void MerkelMain::loadOrderBook() {
    // Se usarmos  OrderBookEntry order1 da erro porque como tem construtor, precisamos passar informações.
    // Usamos "{}" para indicar que é uma construtora.

    /*
    Exemplo:
        Classe (molde):
        ├── price
        ├── amount
        ├── timestamp
        ├── product
        └── orderType

        Objeto obe1 (criado do molde):
        ├── price = 0.02
        ├── amount = 7.4
        ├── timestamp = "2020/03/17"
        ├── product = "BTC/USD"
        └── orderType = bid

        Vetor orders:
        [ obe1, obe2, obe3, ... ]
    */


    // Se fosse "orders.push_back" só seria acessável dentro da função. Precisa colocar em um escopo de objeto.
    orders.push_back(OrderBookEntry{2000,
                                    0.02,
                                    "2020/03/17 17:01:24.884492",
                                    "BTC/USD",
                                    OrderBookType::bid}
                    );


    orders.push_back(OrderBookEntry{4000,
                                    0.02,
                                    "2020/03/17 17:01:24.884492",
                                    "BTC/USD",
                                    OrderBookType::bid}
                    );
}

