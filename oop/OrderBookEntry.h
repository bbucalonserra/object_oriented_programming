#pragma once //"only include this file once. to not include header files multiple times."

#include <string>

enum class OrderBookType{bid, ask};

class OrderBookEntry {
    // Can access data from outside the class, therefore, using main().
    public:
    
        // A constructor is a block of code or a special function that runs when the object is instantiated.
        // When we create this object, we need to pass thhis data.
        // É o construtor porque é uma função que tem o mesmo nome da classe e sem nenhum return.
        // Aqui, define que quando alguem criar um objeto, ele PRECISA RECEBER estes cincos valores com esses data types.
        // São variáveis temporárias, que recebem os valores quando você cria o objeto.
        // "Minha função precisa receber estas informações."
        OrderBookEntry (double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType
        );
        
        // São os espaços de memória permanentes dentro do objeto. É aqui que o valor realmente fica guardado.
        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType; // Enum, it's above.
};