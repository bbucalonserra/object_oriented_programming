#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "Candlestick.h"

/*
NOTE: 
    - Repare que pegamos todos os métodos de OrderBook.h e aplicamos aqui. Eles são identicos, só adicionamos o "{}".
    - Sempre colocamos a classe antes do nome do método (OrderBook::getKnownProducts).
*/

// Constructor. Reading a CSV datafile.
OrderBook::OrderBook(std::string filename) {
    orders = CSVReader::readCSV(filename);
};

// Retorna um vetor, que dentro do vetor so aceita strings.
std::vector<std::string> OrderBook::getKnownProducts() {
    std::vector<std::string> products;

    // Mapa tem sempre uma key e um value. Ele armazena pres de chave valor
    // Cada elemento do mapa é um par: std::pair<const Key, T>, sendo que key pode ser std::string, int, etc. T é tipo associado.
    // Modifiers dele são insert e erase (são metodos do map).
    // std::map é uma classe da biblioteca padrão do C++ (assim como iostream, vector, map, etc.).
    std::map<std::string, bool> prodMap;

    for(OrderBookEntry& e :orders) {
        prodMap[e.product] = true;
    }

    for (auto const& e : prodMap) {
        products.push_back(e.first);
    }

    return products;
};

// Retorna um vetor, que só retorna um vetor de OrderBookEntry, que é a função construtora da classe OrderBookEntry.
// Ou seja, ele retorna um objeto do tipo OrderBookEntry (que por sua vez retorna um "obe", que tem aquelas caracteristicas de cada linha).
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                std::string product,
                                                std::string timestamp) 
{
    std::vector<OrderBookEntry> orders_sub;

    for(OrderBookEntry& e :orders) {
        if(e.orderType == type && e.product == product && e.timestamp == timestamp) {
            orders_sub.push_back(e);
        }
    }

    return orders_sub;
};

//
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders) {
    double max = orders[0].price;

    for(OrderBookEntry& e: orders) {
        if (e.price > max) {
            max = e.price;
        }
    }
    return max;
};

//
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders) {
    double min = orders[0].price;

    for(OrderBookEntry& e: orders) {
        if (e.price < min) {
            min = e.price;
        }
    }
    return min;
};

// Pega a ordem mais recente.
std::string OrderBook::getEarliestTime() {
    return orders[0].timestamp;
};

std::string OrderBook::getNextTime(std::string timestamp) {
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders) {
        if (e.timestamp > timestamp) {
            next_timestamp = e.timestamp;
            break;
        }
    }

    if(next_timestamp == "") {
        next_timestamp = orders[0].timestamp;
    }
    
    return next_timestamp;
};

void OrderBook::insertOrder(OrderBookEntry & order) {
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
};

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp) {
    //
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                product,
                                                timestamp);

    //                                            
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                product,
                                                timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales;

    //
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);

    //
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    // for ask in asks:
    std::cout << "max ask " << asks[asks.size()-1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size()-1].price << std::endl;

    //
    for(OrderBookEntry& ask : asks) {
        for (OrderBookEntry& bid : bids) {
            if (bid.price >= ask.price) {

                // 
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                if (bid.username == "simuser") {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;;
                }

                if (ask.username == "simuser") {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;;
                }

                // 
                if (bid.amount == ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }

                //
                if (bid.amount > ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount;
                    break;
                }

                //
                if (bid.amount < ask.amount && bid.amount > 0) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount;
                    bid.amount = 0;
                    continue; // Ir para proxima bid.
                }
            }
        }
    }
    return sales;
};


// Function to compute the candlesticks. 
// It's a vector (called std::vector<Candlestick>)
// It's from the OrderBook class, called computeCandlesticks.
// It needs a vector std::vector<OrderBookEntry> as parameter.
// & = reference, it will use the original list that is already in memory, does not need to copy.
std::vector<Candlestick> OrderBook::computeCandlesticks(std::vector<OrderBookEntry>& orders){
    std::vector<Candlestick> candlesticks;

    if (orders.size() == 0) return candlesticks;

    double open = orders[0].price;
    double close = orders[orders.size() - 1].price;

    double high = orders[0].price;
    double low = orders[0].price;

    for (OrderBookEntry& e : orders) 
        {
            // Se o preço desta ordem for maior que o nosso recorde 'high', atualizamos o recorde
            if (e.price > high) high = e.price;
            
            // Se o preço desta ordem for menor que o nosso recorde 'low', atualizamos o recorde
            if (e.price < low) low = e.price;
        }

    Candlestick candle{orders[0].timestamp, open, high, low, close};

    candlesticks.push_back(candle);

    return candlesticks;
}