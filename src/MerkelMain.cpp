// $ g++ --std=c++11 OrderBookEntry.cpp MerkelMain.cpp main.cpp CSVReader.cpp
// $ ./a.exe

// IMPORTANTE: quando é uma funçao, usamos obj.metodo. Quando é static, ou seja, nao retorna nada, usamos Classe::metodo()
// Um é pertencente a classe, outro ao objeto atribuido a classe.
// Usamos o ":" e nao "." quando o atributo (variavel) ou metodo é estatico a classe.


#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

MerkelMain::MerkelMain() {


}

void MerkelMain::init() {
    // loadOrderBook();
    int input;
    currentTime = orderBook.getEarliestTime();
    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

// void MerkelMain::loadOrderBook() {
//     orders = CSVReader::readCSV("20200317.csv"); 
//     //orders = CSVReader::readCSV("big_data.csv"); 
    
// }

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

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp() {
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats() {

    // Aqui vai printar quando entrar no menu e escolher 2: "Print exchange stats"
    for (std::string const p : orderBook.getKnownProducts()) {

        std::cout << "Product: " << p << std::endl;

        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);

        std::cout << "Asks seen: " << entries.size() << std::endl;

        std::cout << "Max asks: " << OrderBook::getHighPrice(entries) << std::endl;

        std::cout << "Min asks: " << OrderBook::getLowPrice(entries) << std::endl;
    }

    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void MerkelMain::enterOffer() {
    std::cout << "Mark and offer - enter the amount " << std::endl;
}

void MerkelMain::enterBid() {
    std::cout << "Make a bid - enter the amount" << std::endl;
}

void MerkelMain::printWallet() {
    std::cout << "Your wallet is empty. " << std::endl;
}
        
void MerkelMain::gotoNextTimeframe() {
    std::cout << "Going to next time frame. " << std::endl;

    // Aqui a cada vez que escolhe a opção 6, ele vai para o próximo.
    currentTime = orderBook.getNextTime(currentTime);
}
 
int MerkelMain::getUserOption() {
    int userOption;

    std::cout << "Type in 1-6" << std::endl;
    std::cin >> userOption;
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption) {
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterOffer();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        printWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }       
}