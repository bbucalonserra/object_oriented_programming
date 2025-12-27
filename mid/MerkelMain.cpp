// $ g++ --std=c++11 OrderBookEntry.cpp MerkelMain.cpp main.cpp CSVReader.cpp
// $ ./a.exe

// IMPORTANT: When dealing with an instance function, we utilise obj.method(). 
// When the method is static (meaning it does not return an instance-specific value), 
// we use Class::method(). One belongs to the class itself, whilst the other 
// belongs to the specific object assigned to that class.
// We utilise the ":" operator rather than "." when the attribute (variable) 
// or method is static to the class.


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

/** * This constructor serves to initialise the class instance, 
 * primarily establishing the seed for pseudo-random number generation 
 * which is essential for subsequent user-related operations.
 */
MerkelMain::MerkelMain() {
    // Generate random number.
    std::srand(std::time(nullptr));
}

/** * Acts as the primary entry point for the application's execution flow; 
 * it orchestrates the initial data loading and maintains the principal 
 * operational loop for user authentication and menu navigation.
 */
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

/** * Provides a formal visual interface for the user, enumerating all 
 * available system commands and displaying the current temporal 
 * state of the exchange platform.
 */
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

    // 7. Candlestick
    std::cout << "7: View Candlestick Stats" << std::endl;

    // 8. Deposit Money.
    std::cout << "8: Deposit money" << std::endl;

    // 9. Withdraw money.
    std::cout << "9: Withdraw money" << std::endl;

    // 10. Print Trade History.
    std::cout << "10: Print trade history." << std::endl;

    // 11. Orubt Trading Stats.
    std::cout << "11: Print trading stats." << std::endl;

    // 12. Simulate trending,
    std::cout << "12: Simulate Trading." << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

/** * Offers a concise pedagogical guide to assist the user in 
 * understanding the fundamental objectives of the trading simulation.
 */
void MerkelMain::printHelp() {
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

/** * Conducts a comprehensive analysis of the current market state, 
 * aggregating and presenting statistics such as price extrema 
 * for all identified products within the order book.
 */
void MerkelMain::printMarketStats() {

    // Output is displayed upon accessing the menu and selecting Option 2: "Print exchange stats"
    for (std::string const p : orderBook.getKnownProducts()) {

        std::cout << "Product: " << p << std::endl;

        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);

        std::cout << "Asks seen: " << entries.size() << std::endl;

        std::cout << "Max asks: " << OrderBook::getHighPrice(entries) << std::endl;

        std::cout << "Min asks: " << OrderBook::getLowPrice(entries) << std::endl;
    }

}

/** * Facilitates the creation of a sell order (ask); it validates user input, 
 * verifies the adequacy of wallet funds, and formally registers the order 
 * within the system's ledger.
 */
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

/** * Orchestrates the submission of a buy order (bid) by parsing user specifications 
 * and ensuring that the internal wallet possesses the requisite liquidity 
 * to support the intended transaction.
 */
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

/** * Retrieves and displays the current asset balances associated with 
 * the authenticated user's wallet in a human-readable format.
 */
void MerkelMain::printWallet() {
    std::cout << userWallets[currentUser].toString() << std::endl;
}
        
/** * Advances the simulation to the subsequent temporal interval; it triggers 
 * the matching engine to execute trades and ensures all successful transactions 
 * are persistently logged and reflected in the user's financial balance.
 */
void MerkelMain::gotoNextTimeframe() {
    std::cout << "Going to next time frame. " << std::endl;

    // 1. Process trades for each individual product
    for (std::string const& p : orderBook.getKnownProducts()) {
        std::cout << "Matching " << p << std::endl;
        
        // Execute the OrderBook matching engine
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;

        for (OrderBookEntry& sale : sales) {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            
            // 2. Verify if the sale pertains to the currently authenticated user
            if (sale.username == "simuser") { // "simuser" acts as the identifier for the user's orders
                
                // Update the wallet state in RAM
                userWallets[currentUser].processSale(sale);
                
                // 3. PERSISTENCE: Log the transaction into the CSV file
                std::ofstream file("transactions.csv", std::ios::app);
                if (file.is_open()) {
                    file << currentTime << "," 
                         << currentUser << "," 
                         << sale.product << "," 
                         << (sale.orderType == OrderBookType::ask ? "ask" : "bid") << "," 
                         << sale.price << "," 
                         << sale.amount << std::endl;
                    file.close();
                }
            }
        }
    }

    // 5. Increment the temporal state
    currentTime = orderBook.getNextTime(currentTime);
}
 
/** * Captures and sanitises numerical input from the user console, 
 * employing error-handling mechanisms to manage non-integer responses 
 * during the selection process.
 */
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
        // No action required for erroneous input; handled by default value.
    }

    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

/** * Dispatches the appropriate logic or sub-routine based on the 
 * numerical option selected by the user, serving as the primary 
 * controller for menu-driven actions.
 */
void MerkelMain::processUserOption(int userOption) {
    if (userOption == 0) // Handle erroneous input
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
    if (userOption == 8) {
        depositMoney();}
    
    if (userOption == 9) {
        withdrawMoney();  
    }

    if (userOption == 10) {
        printTradeHistory();
    }

    if (userOption == 11) {
        printTradingStats();
    }

    if (userOption == 12) {
        simulateTrading();
    }
}

/** * Computes and displays OHLC (Open, High, Low, Close) price data 
 * for a specific asset over a requested timeframe, facilitating 
 * technical analysis for the user.
 */
void MerkelMain::printCandlestickStats() {
    std::cout << "Select Product (e.g.: ETH/BTC): " << std::endl;
    std::string product;
    std::getline(std::cin, product);

    std::cout << "Enter Date (YYYY/MM/DD HH:MM:SS) or press Enter for current time: " << std::endl;
    std::string inputDate;
    std::getline(std::cin, inputDate);

    // Should the user press Enter, the system time (2020) is utilised.
    // If a specific date is entered, the system searches accordingly.
    std::string searchTime = inputDate.empty() ? currentTime : inputDate;

    std::vector<OrderBookEntry> entries = orderBook.getOrders(
        OrderBookType::ask, 
        product, 
        searchTime
    );

    std::vector<Candlestick> candles = OrderBook::computeCandlesticks(entries);

    if (candles.empty()) {
        std::cout << "No entries found for " << searchTime << std::endl;
    } else {
        Candlestick& c = candles[0];
        std::cout << "=== CANDLESTICK for " << product << " ===" << std::endl;
        std::cout << "Date:  " << searchTime << std::endl;
        std::cout << "Open:  " << c.open << std::endl;
        std::cout << "High:  " << c.high << std::endl;
        std::cout << "Low:   " << c.low << std::endl;
        std::cout << "Close: " << c.close << std::endl;
    }
}

/** * Manages the creation of a new user profile; it collects personal details, 
 * secures the chosen password via cryptographic hashing, and persists 
 * the new account data to the external database.
 */
void MerkelMain::registerUser() {
    std::cout << "--- REGISTRATION ---" << std::endl;

    // Initialise local variables for name, email, and password.
    std::string fullName, email, password;

    // Use getline to capture the full name from standard input.
    // This ensures that names containing spaces are read correctly until a newline is encountered.
    std::cout << "Full name: " << std::endl;
    std::getline(std::cin, fullName);

    // Capture email address using the same logic.
    std::cout << "Email: " << std::endl;
    std::getline(std::cin, email);

    // Capture password input.
    std::cout << "Select a password: ";
    std::getline(std::cin, password);

    // Generate a unique random userID.
    std::string userId = std::to_string(98500000 + rand() % 90000000);

    // Implement password protection via cryptographic hashing.
    std::hash<std::string> hasher;
    size_t hashedPW = hasher(password);
    
    // Store user data in the internal mapping.
    idToName[userId] = fullName;

    // Verify if the user credentials already exist within the map structure.
    // The 'find' method returns an iterator pointing to the element or 'end()' if not found.
    if (userDatabase.find(email) == userDatabase.end()) {
            // Register user in the database.
            userDatabase[userId] = std::to_string(hashedPW);

            // Notify user of successful registration.
            std::cout << "The user "<< fullName << "is now registered." << std::endl;
            std::cout << "Your login ID is " << userId << std::endl;
    } else {
        std::cout << "The user is already registered." << std::endl;
    }

    // Initialise wallet with a standard starting balance.
    Wallet newWallet;
    newWallet.insertCurrency("USDT", 10.0);
    userWallets[userId] = newWallet;


    // Persist registration data to the CSV file.
    std::ofstream file("users.csv", std::ios::app);
    if (file.is_open()) {
        file << userId << "," << fullName << "," << email << "," << std::to_string(hashedPW) << std::endl;
        file.close();
    }
}

/** * Handles the authentication process by verifying the provided user ID 
 * and password against stored records, subsequently granting access 
 * and loading the user's specific financial state.
 */
void MerkelMain::login() {
    std::cout << "--- LOGIN ---" << std::endl;
    std::cout << "userId: " << std::endl;

    // Initialise local variables for identification and credentials.
    std::string idInput, passwordInput;

    // Capture User ID input, handling whitespace appropriately.
    std::getline(std::cin >> std::ws, idInput);

    // Prompt for password entry.
    std::cout << "Password: ";
    
    // Capture password input.
    std::getline(std::cin, passwordInput);

    // Hash the input password for secure comparison.
    std::hash<std::string> hasher;
    std::string hashedInput = std::to_string(hasher(passwordInput));

    // Validate credentials against the internal database.
    if (userDatabase.find(idInput) != userDatabase.end() && userDatabase[idInput] == hashedInput) {
            // Set the global current user identifier.
            currentUser = idInput;

            // Load the corresponding wallet data.
            loadUserWallet();

            // Retrieve and display a personalised welcome message.
            std::string name = idToName[idInput];

            std::cout << "Welcome " << name << "!" << std::endl;
    } else {
        std::cout << "Invalid user ID or password. Try again." << std::endl;
    }
};

/** * Parses the persistent user data file to populate the application's 
 * internal memory with registered accounts and their respective 
 * wallet configurations upon system startup.
 */
void MerkelMain::loadUsers() {
    // Open the user database file.
    std::ifstream file("users.csv");

    std::string line;

    if (!file.is_open()) return;

    // Iterate through each line of the CSV file.
    while (std::getline(file, line)) {
        // Tokenise the line based on the comma delimiter.
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        
        // Ensure the record contains the necessary fields.
        if (tokens.size() >= 4) {
            // Assign identifiers, names, and hashes from tokens.
            std::string id = tokens[0];
            std::string name = tokens[1];
            std::string hash = tokens[3];

            // Initialise a wallet for the loaded user.
            Wallet loadedWallet;

            // Check for additional wallet metadata (5th token).
            if (tokens.size() == 5) {
                // Parse the wallet string (e.g., "BTC:10.5,ETH:2.0").
                std::string walletData = tokens[4];
                
                // 1. Separate currency pairs by comma.
                std::vector<std::string> currenciesList = CSVReader::tokenise(walletData, ',');
                
                for (std::string& currData : currenciesList) {
                    // 2. Separate currency type from amount by colon.
                    std::vector<std::string> pair = CSVReader::tokenise(currData, ':');
                    if (pair.size() == 2) {
                        std::string type = pair[0];
                        double amount = std::stod(pair[1]);
                        
                        // 3. Populate the user's wallet with existing funds.
                        loadedWallet.insertCurrency(type, amount);
                    }
                }
            } else {
                loadedWallet.insertCurrency("USDT", 10.0); // Default initial balance.
            }

            // Map the ID to the user's details and wallet.
            userDatabase[id] = hash;
            idToName[id] = name;
            userWallets[id] = loadedWallet;
        }
    }
};

/** * Permits the manual augmentation of a user's wallet balance; 
 * it verifies that the currency is valid and logs the deposit 
 * event for historical tracking.
 */
void MerkelMain::depositMoney() {
    // Identify currency type (e.g., ETH, BTC, USDT, DOGE).
    std::cout << "Currency (ETH, BTC, USDT, DOGE): ";
    std::string type; std::cin >> type;
    
    // Validate currency against supported crypto types.
    CryptoType crypto = stringToCrypto(type);
    if (crypto == CryptoType::UNKNOWN) {
        std::cout << "Invalid currency! Only ETH, BTC, USDT, DOGE allowed." << std::endl;
        return;
    }

    // Capture deposit quantity.
    std::cout << "Quantity: ";
    double amount; std::cin >> amount;

    // Update the internal wallet balance.
    userWallets[currentUser].insertCurrency(type, amount);
    
    double currentBalance = userWallets[currentUser].getCurrencyBalance(type);

    // Log the event to 'wallet_activity.csv' for persistence.
    std::ofstream file("wallet_activity.csv", std::ios::app);
    if (file.is_open()) {
        file << currentUser << "," << "deposit" << "," << type << "," << amount << "," << currentBalance <<std::endl;
        file.close();
    }

    // Confirm transaction completion.
    std::cout << "The deposit is completed. New balance: " << currentBalance << " " << type << std::endl;
}

/** * Retrieves and presents a chronological list of the most recent 
 * transactions associated exclusively with the logged-in user.
 */
void MerkelMain::printTradeHistory() {
    std::ifstream file("transactions.csv");
    std::string line;
    std::vector<std::string> history;
    while (std::getline(file, line)) {
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        if (tokens[1] == currentUser) { // Filter by the active user
            history.push_back(line);
        }
    }
    std::cout << "--- Last 5 transactions ---" << std::endl;
    int start = (history.size() > 5) ? history.size() - 5 : 0;
    for (int i = start; i < history.size(); ++i) {
        std::cout << history[i] << std::endl;
    }
}

/** * Compiles and reports quantitative metrics of a user's trading activity, 
 * such as the aggregate volume of bids and asks, either for the 
 * entire portfolio or a specific product.
 */
void MerkelMain::printTradingStats() {
    double totalSpent = 0;
    int totalBids = 0, totalAsks = 0;
    
    // Facilitate specific product filtering.
    std::cout << "Enter product for specific stats (or 'all'): ";
    std::string targetProduct;
    std::getline(std::cin >> std::ws, targetProduct);

    std::ifstream file("transactions.csv");
    std::string line;
    
    while (std::getline(file, line)) {
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        if (tokens.size() >= 6 && tokens[1] == currentUser) {
            std::string product = tokens[2];
            std::string type = tokens[3];
            double price = std::stod(tokens[4]);
            double amount = std::stod(tokens[5]);

            // Apply product filter or aggregate all records.
            if (targetProduct == "all" || targetProduct == product) {
                if (type == "bid") {
                    totalBids++;
                    totalSpent += price * amount;
                } else {
                    totalAsks++;
                }
            }
        }
    }

    std::cout << "\n=== Trading Stats [" << targetProduct << "] ===" << std::endl;
    std::cout << "User: " << idToName[currentUser] << std::endl;
    std::cout << "Number of Bids: " << totalBids << std::endl;
    std::cout << "Number of Asks: " << totalAsks << std::endl;
    std::cout << "Total money spent (Bids): " << totalSpent << std::endl;
    std::cout << "==============================" << std::endl;
}

/** * Facilitates the removal of funds from the user's wallet; 
 * it ensures that the balance is sufficient for the request 
 * before updating both memory and persistent storage.
 */
void MerkelMain::withdrawMoney() {
    std::cout << "Currency (ETH, BTC, USDT, DOGE): ";
    std::string type; std::cin >> type;
    
    if (stringToCrypto(type) == CryptoType::UNKNOWN) {
        std::cout << "Invalid currency!" << std::endl;
        return;
    }

    std::cout << "Quantity: ";
    double amount; std::cin >> amount;

    // 1. Attempt to deduct the amount from internal memory.
    if (userWallets[currentUser].removeCurrency(type, amount)) {
        
        // 2. Retrieve updated balance for logging purposes.
        double currentBalance = userWallets[currentUser].getCurrencyBalance(type);

        // 3. Persist the activity including the current balance column.
        std::ofstream file("wallet_activity.csv", std::ios::app);
        if (file.is_open()) {
            file << currentUser << "," << "withdraw" << "," << type << "," << amount << "," << currentBalance << std::endl;
            file.close();
        }
        std::cout << "Withdrawal completed. New balance: " << currentBalance << " " << type << std::endl;
    } else {
        std::cout << "Insufficient funds!" << std::endl;
    }
}

/** * Converts a string representation of a cryptocurrency into its 
 * corresponding enumerated type for structured internal processing.
 */
CryptoType MerkelMain::stringToCrypto(std::string s) {
    if (s == "ETH") return CryptoType::ETH;
    if (s == "BTC") return CryptoType::BTC;
    if (s == "USDT") return CryptoType::USDT;
    if (s == "DOGE") return CryptoType::DOGE;
    return CryptoType::UNKNOWN;
}

/** * Executes an automated trading scenario using historical or simulated timestamps; 
 * it generates mock market entries to stress-test the persistence of 
 * the order book and the accuracy of wallet updates.
 */
void MerkelMain::simulateTrading()
{
    // Request a backdated timestamp for the simulation (Requirement: Filter Task).
    std::cout << "Please enter the backdated time for simulation." << std::endl;
    std::cout << "Format: YYYY/MM/DD HH:MM:SS (e.g., 2024/05/15 10:30:00): " << std::endl;
    
    std::string backdatedTime;
    std::getline(std::cin >> std::ws, backdatedTime);

    // Perform basic format validation.
    if (backdatedTime.length() < 19) {
        std::cout << "Invalid format. Using default: 2024/01/01 12:00:00" << std::endl;
        backdatedTime = "2024/01/01 12:00:00";
    }

    std::cout << "Starting simulation... please wait." << std::endl;

    // 2. Prepare real-world system time for mixed-mode simulation.
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", now);
    std::string realTime = std::string(buf);

    std::vector<std::string> products = orderBook.getKnownProducts();
    
    // Open necessary files for data logging.
    std::ofstream marketFile("new_bid_ask.csv", std::ios::app);
    std::ofstream walletFile("wallet_activity.csv", std::ios::app);

    if (marketFile.is_open()) {
        for (const std::string& p : products) {
            for (int i = 0; i < 5; ++i) {
                // Alternate between system time and user-provided backdated time.
                std::string selectedTime = (i % 2 == 0) ? realTime : backdatedTime;
                double price = 100.0 + i;
                double amount = 0.5;

                // A. Insert into system RAM.
                OrderBookEntry entry{price, amount, selectedTime, p, OrderBookType::ask, currentUser};
                orderBook.insertOrder(entry);

                // B. Record in the market data file (Requirement: Task 4).
                marketFile << currentUser << "," << selectedTime << "," << p << ",ask," << price << "," << amount << std::endl;

                // C. Implement Wallet Persistence Logic (Requirement: Task 5).
                // Deducing the base currency (e.g., ETH from ETH/BTC).
                std::vector<std::string> currs = CSVReader::tokenise(p, '/');
                std::string currencyToDeduct = currs[0];

                // Attempt to remove funds from memory and log the success.
                if (userWallets[currentUser].removeCurrency(currencyToDeduct, amount)) {
                    if (walletFile.is_open()) {
                        double newBalance = userWallets[currentUser].getCurrencyBalance(currencyToDeduct);
                        walletFile << currentUser << "," 
                                   << "simulated_trade" << "," 
                                   << currencyToDeduct << "," 
                                   << amount << "," 
                                   << newBalance << std::endl;
                    }
                }
            }
        }
        marketFile.close();
        if (walletFile.is_open()) walletFile.close();

        std::cout << ">>> Simulation completed successfully!" << std::endl;
        std::cout << "Market data saved to new_bid_ask.csv" << std::endl;
        std::cout << "Wallet activity logged to wallet_activity.csv" << std::endl;
        std::cout << "You can now check candlesticks for date: " << backdatedTime << std::endl;
    } else {
        std::cout << "Error: Could not open files for saving simulation data." << std::endl;
    }
}

/** * Reconstructs the current user's wallet state by auditing historical 
 * activity logs from the wallet CSV, ensuring that financial 
 * continuity is maintained across sessions.
 */
void MerkelMain::loadUserWallet()
{
    std::ifstream walletFile("wallet_activity.csv");
    std::string line;

    if (walletFile.is_open())
    {
        while (std::getline(walletFile, line))
        {
            std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
            if (tokens.size() == 5)
            {
                std::string userId = tokens[0];
                std::string type = tokens[1]; // includes deposit, simulated_trade, etc.
                std::string currency = tokens[2];
                double amount = std::stod(tokens[3]);

                // Only process records belonging to the currently authenticated user.
                if (userId == currentUser)
                {
                    if (type == "deposit")
                    {
                        userWallets[userId].insertCurrency(currency, amount);
                    }
                    else if (type == "simulated_trade" || type == "withdraw")
                    {
                        userWallets[userId].removeCurrency(currency, amount);
                    }
                }
            }
        }
        walletFile.close();
        std::cout << "Wallet loaded from persistent storage." << std::endl;
    }
}