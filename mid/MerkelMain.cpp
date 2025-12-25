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
#include "Wallet.h"
#include <limits>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <fstream>

MerkelMain::MerkelMain() {
    // Generate random number.
    std::srand(std::time(nullptr));
}

void MerkelMain::init() {
    // loadOrderBook();
    int input;
    currentTime = orderBook.getEarliestTime();

    // Load users to check if the user is available.
    loadUsers();


    while(true) {
        if (currentUser == "") {
            std::cout << "1: Login\n2: Register" << std::endl;
            input = getUserOption(1);
            if (input == 1) login();
            if (input == 2) registerUser();
        } else {
        printMenu();
        input = getUserOption(0);
        processUserOption(input);
        }
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

    // Candlestick
    std::cout << "7: View Candlestick Stats" << std::endl;

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

}

void MerkelMain::enterAsk() {
    std::cout << "Mark an ask - enter the amount: product, price, amount, eg <ETH/BTC, 200, 0.5>" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3) {
        std::cout << "MerkelMain::enterAsk Bad input!" << std::endl;
    }

    else {
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );

            obe.username = "simuser";

            if(userWallets[currentUser].canFulfillOrder(obe)) {
                std::cout << "Wallet looks goods. " << std::endl;
                orderBook.insertOrder(obe);
            }

            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }

        } catch (const std::exception& e) 
        {
            std::cout << "MerkelMain::enterAsk Bad input " << std::endl;
        }
    } 
}

void MerkelMain::enterBid() {
    std::cout << "Mark an bid - enter the amount: product, price, amount, eg <ETH/BTC, 200, 0.5>" << std::endl;
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3) {
        std::cout << "MerkelMain::enterBid Bad input!" << std::endl;
    }

    else {
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );

            obe.username = "simuser";

            if(userWallets[currentUser].canFulfillOrder(obe)) {
                std::cout << "Wallet looks goods. " << std::endl;
                orderBook.insertOrder(obe);
            }

            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }

        } catch (const std::exception& e)
        {
            std::cout << "MerkelMain::enterBid Bad input " << std::endl;
        }
    } 
}

void MerkelMain::printWallet() {
    std::cout << userWallets[currentUser].toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe() {
    std::cout << "Going to next time frame. " << std::endl;

    //
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry& sale : sales) {
        std::cout << "Sale price: " << sale.amount << " amount " << sale.amount << std::endl;
        if (sale.username == "simuser") {
            //update wallet.
            userWallets[currentUser].processSale(sale);   
        }
    }


    // Aqui a cada vez que escolhe a opção 6, ele vai para o próximo.
    currentTime = orderBook.getNextTime(currentTime);
}
 
int MerkelMain::getUserOption(bool isRegistration) {
    int userOption=0;
    std::string line;

    if (isRegistration) {
        std::cout << "Type in 1 or 2." << std::endl;
    } else {
    std::cout << "Type in 1-6" << std::endl; 
    }

    std::getline(std::cin, line);

    try {
    userOption = std::stoi(line);
    } catch(const std::exception& e) {
        // We don't need to do nothing here.
    }

    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
        enterAsk();
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
    if (userOption == 7) {
        printCandlestickStats();
    }
}

void MerkelMain::printCandlestickStats() {
    std::cout << "Select Product (e.g.: ETH/BTC): " << std::endl;
    std::string product;
    std::getline(std::cin, product);

    // Pegamos as ordens do mercado atual (Ask) para esse produto
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        OrderBookType::ask, 
        product, 
        currentTime
    );

    // Chamamos a sua função da Task 1!
    std::vector<Candlestick> candles = OrderBook::computeCandlesticks(entries);

    // Mostramos o resultado na tela
    if (candles.empty()) {
        std::cout << "Nenhuma ordem encontrada para este produto/horario." << std::endl;
    } else {
        Candlestick& c = candles[0]; // Pegamos o resumo gerado
        std::cout << "=== CANDLESTICK para " << product << " ===" << std::endl;
        std::cout << "Date:  " << c.date << std::endl;
        std::cout << "Open:  " << c.open << std::endl;
        std::cout << "High:  " << c.high << std::endl;
        std::cout << "Low:   " << c.low << std::endl;
        std::cout << "Close: " << c.close << std::endl;
        std::cout << "==============================" << std::endl;
    }
}

void MerkelMain::registerUser() {
    std::cout << "--- REGISTRATION ---" << std::endl;

    // Create a internal variable for name, email and password.
    std::string fullName, email, password;

    // Get line to read fullName. The cin is user input from keyboard.
    // The get line reads until "\n" is found. It will get everything until the user presses enter.
    // E.g.: Trisha Amstel, std::cin >> fullName reads only "Trisha", with the get line, it will read the next after \n.
    std::cout << "Full name: " << std::endl;
    std::getline(std::cin, fullName);

    // Email. Same idea as before.
    std::cout << "Email: " << std::endl;
    std::getline(std::cin, email);

    // Password. Same idea as before.
    std::cout << "Select a password: ";
    std::getline(std::cin, password);

    // Generate random userId.
    std::string userId = std::to_string(98500000 + rand() % 90000000);

    // Protect pasword by using hash.
    std::hash<std::string> hasher;
    size_t hashedPW = hasher(password);
    
    // Save in map.
    idToName[userId] = fullName;

    // Check if the input of user is already available in the map ("dictionary").
    // The find is a method from several data structures (e.g. map, set, vector). It returns an iterator.
    // Here, it will "point" to where it finds the value in the map, if not, will point to the void (called end()).
    if (userDatabase.find(email) == userDatabase.end()) {
            // Add in map.
            userDatabase[userId] = std::to_string(hashedPW);

            // Print success.
            std::cout << "The user "<< fullName << "is now registered." << std::endl;
            std::cout << "Your login ID is " << userId << std::endl;
    } else {
        std::cout << "The user is already registered." << std::endl;
    }

    // Add money to wallet.
    Wallet newWallet;
    newWallet.insertCurrency("BTC", 10.0);
    userWallets[userId] = newWallet;


    // Save to file.
    std::ofstream file("users.csv", std::ios::app);
    if (file.is_open()) {
        file << userId << "," << fullName << "," << email << "," << std::to_string(hashedPW) << std::endl;
        file.close();
    }
}

void MerkelMain::login() {
    std::cout << "--- LOGIN ---" << std::endl;
    std::cout << "userId: " << std::endl;

    // Create a internal variable for fullName.
    std::string idInput, passwordInput;

    // Get line to read fullName. The cin is user input from keyboard.
    // The get line reads until "\n" is found. It will get everything until the user presses enter.
    // E.g.: Trisha Amstel, std::cin >> fullName reads only "Trisha", with the get line, it will read the next after \n.
    std::getline(std::cin >> std::ws, idInput);

    // Print texto to add password.
    std::cout << "Password: ";
    
    // Create a internal variable for password.
    std::getline(std::cin, passwordInput);

    //
    std::hash<std::string> hasher;
    std::string hashedInput = std::to_string(hasher(passwordInput));

    // Check if the input of user is available in the map ("dictionary").
    // The find is a method from several data structures (e.g. map, set, vector). It returns an iterator.
    // Here, it will "point" to where it finds the value in the map, if not, will point to the void (called end()).
    if (userDatabase.find(idInput) != userDatabase.end() && userDatabase[idInput] == hashedInput) {
            // Add username currentUser. 
            currentUser = idInput;

            //
            std::string name = idToName[idInput];

            // Print success.
            std::cout << "Welcome " << name << "!" << std::endl;
    } else {
        std::cout << "Invalid user ID or password. Try again." << std::endl;
    }
};

void MerkelMain::loadUsers() {
    //
    std::ifstream file("users.csv");

    //
    std::string line;

    //
    if (!file.is_open()) return;

    //
    while (std::getline(file, line)) {
            // Using tokenize.
            std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
            
            // Check if all information is properly filled.
            if (tokens.size() == 4) {
                // ID (from tokenize).
                std::string id = tokens[0];
                
                // ID (from tokenize).
                std::string name = tokens[1];
                
                // ID (from tokenize).
                std::string hash = tokens[3];

                // Connect ID with name.
                userDatabase[id] = hash;
                idToName[id] = name;
            
                // Create wallet for loaded users.
                Wallet loadedWallet;
                loadedWallet.insertCurrency("BTC", 10.0); // Standard initial value.
                userWallets[id] = loadedWallet;
            }
        }
};