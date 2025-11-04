#include "OrderBook.h"
#include "CSVReader.h"
#include <map>

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