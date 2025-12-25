#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include "Candlestick.h"

class OrderBook {
    public:
        // Constructor. Reading a CSV datafile.
        OrderBook(std::string filename);

        // Retorna um vetor, que dentro do vetor so aceita strings.
        std::vector<std::string> getKnownProducts();

        // Retorna um vetor, que só retorna um vetor de OrderBookEntry, que é a função construtora da classe OrderBookEntry.
        // Ou seja, ele retorna um objeto do tipo OrderBookEntry (que por sua vez retorna um "obe", que tem aquelas caracteristicas de cada linha).
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                            std::string product,
                                            std::string timestamp
                                    );

        // o símbolo & indica que o vetor é passado por referência, ou seja:
        // - A função não cria uma cópia do vetor.
        // - la usa diretamente o vetor original (o mesmo que foi passado).
        // Sem usar o &, se o vetor tiver 1 milhão de elementos, ele duplica tudo na memória.
        static double getHighPrice(std::vector<OrderBookEntry>& orders);


        //
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        //
        std::string getEarliestTime();

        //
        std::string getNextTime(std::string timestamp);

        //
        void insertOrder(OrderBookEntry & order);

        //
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        // Creating static function (static because it's not needed to create an object to use it.)
        static std::vector<Candlestick> computeCandlesticks(std::vector<OrderBookEntry>& orders);

    private:
        // Para armazenar as ordens. Aqui devemos declarar ele pois cada objeto que criar dessa classe vai ter seu próprio vetor (cada linha ACHO).
        // Logo precisamos declara-lo novamente no .cpp.
        std::vector<OrderBookEntry> orders;
        
};