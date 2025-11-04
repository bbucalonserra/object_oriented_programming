// g++ --std=c++11 main.cpp OrderBookEntry.cpp MerkelMain.cpp
// ./a.exe


// I = in, O = Out, Stream = Stream (entrada e saída).
#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "MerkelMain.h"

int main () {
    MerkelMain app{};
    app.init();
}



    // double price = 5313.420228;
    // double amount = 0.00020075;

    // std::string timestamp ("2020/03/17 17:01:24.884492");
    // std::string product ("BTC/USD");

    // // Create an enum class with two options.
    // enum class orderBookType{bid, ask};

    // // We're using the class created before just like using int before defining something, moreover we select the type of the enum with doulbe ":".
    // // OrderBookType orderType = OrderBookType::ask;


    // std::vector<double> prices;
    // std::vector<double> amounts;
    // std::vector<std::string> timestamps;
    // std::vector<std::string> products;
    // std::vector<orderBookType> orderTypes;

    // prices.push_back(5000.01);
    // amounts.push_back(0.001);
    // timestamps.push_back("BTC/USD");
    // orderTypes.push_back(orderBookType::bid);

    // prices.push_back(10.01);
    // amounts.push_back(0.001);
    // timestamps.push_back("BTC/USD");
    // orderTypes.push_back(orderBookType::ask);


    // // while (true) {
    // //     printMenu();
    // //     int userOption = getUserOption();
    // //     processUserOption(userOption);
    // // }
    // // return 0;

    // // We need only this line to instance the object.
    // // Aqui, o order1 passa a existir na RAM.
    // // Uma classe define um tipo de objeto, e objetos sao instâncias.
    // // A cima, o OrderBookEntry (que tem double etc.) é um "molde"
    // // Ele só passa a existir depois de OrderBookEntry order1.
    // /*
    // O g++ main.cpp -o main compila o código e traduz pra assembly (o ".s"),
    // depois, o ".s" é traduzido para binário (o ".o"), por fim junta
    // tudo isso com bibliotecas formando o ".exe", que contem as informações
    // que o processador entende diretamente. ISSO ESTA ARMAZENADO
    // NO DISCO (HD OU SSD).
    // Quando se executa o programa ("./main"), o sistema operacional (SO) faz algo enorme::
    //     - Copia o binário do disco para a RAM.
    //     - Separa regiões de memória
    //     - Entrega o controle para a CPU
    //     - O CPU encontra o objeto da classe (ex: order1)
    //     - CPU segue o que o compilador disse, ex: "Reserve espaço na stack para um objeto OrderBookEntry".
    // */


    // // Criar vetor para armazenar instancias de uma classe nova.
    // std::vector<OrderBookEntry> orders;


    // // Se usarmos  OrderBookEntry order1 da erro porque como tem construtor, precisamos passar informações.
    // // Usamos "{}" para indicar que é uma construtora.

    // /*
    // Exemplo:
    //     Classe (molde):
    //     ├── price
    //     ├── amount
    //     ├── timestamp
    //     ├── product
    //     └── orderType

    //     Objeto obe1 (criado do molde):
    //     ├── price = 0.02
    //     ├── amount = 7.4
    //     ├── timestamp = "2020/03/17"
    //     ├── product = "BTC/USD"
    //     └── orderType = bid

    //     Vetor orders:
    //     [ obe1, obe2, obe3, ... ]
    // */


    // orders.push_back(OrderBookEntry{2000,
    //                                 0.02,
    //                                 "2020/03/17 17:01:24.884492",
    //                                 "BTC/USD",
    //                                 OrderBookType::bid}
    //                 );


    // orders.push_back(OrderBookEntry{4000,
    //                                 0.02,
    //                                 "2020/03/17 17:01:24.884492",
    //                                 "BTC/USD",
    //                                 OrderBookType::bid}
    //                 );

    // for (OrderBookEntry& order : orders) {
    //     std::cout << "The price is " << order.price << std::endl;
    // }

    // for (unsigned int i = 0; i < orders.size(); ++i) {
    //     std::cout << "The price is " << orders[i].price << std::endl;
    // }