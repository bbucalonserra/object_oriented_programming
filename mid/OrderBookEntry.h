// Garantir que este arquivo de cabeçalho (.h) só seja incluído uma vez durante a compilação.
#pragma once
#include <string>

// Criando um tipo de dado chamado "OrderBookType" (que é uma classe), só aceitando bid, ask e unkonwn.
// Nao é uma classe de verdade, o enum class serve apenas para criar um tipo com valores fixos nomeados.
// Ele não cria memória para atributos, nem métodos.
enum class OrderBookType{bid, ask, unknown, asksale, bidsale};

class OrderBookEntry
{
    public:

        // Aqui, estamos dizendo: “Quando alguém quiser criar um OrderBookEntry, ele tem que me entregar 5 valores.”
        // Literalmente apenas parâmetros.
        // Isto é uma DECLARAÇÃO DE MÉTODO DENTRO DA CLASSE. Como já está dentro de OrderBookEntry, compilador já sabe que é daquela classe.
        OrderBookEntry( double _price, // O "_" não significa nada, é apenas representativo.
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType,
                        std::string username="dataset"); // se nao especificarmos, default dataset.

        // Existe uma função chamada stringToOrderBookType, que pertence à classe OrderBookEntry, recebe uma string e devolve um OrderBookType. 
        // Ela é estática (não depende de objeto).
        // OrderBookType é o tipo (no caso enum que veio da classe OrderBookType), se chama stringToOrderBookType e deve receber uma string s.
        static OrderBookType stringToOrderBookType(std::string s); // static significa que nao precisa criar objeto antes (o "objeto_xx = xxx")
        // Aqui já são as variáveis internas da classe. São os espaços de memória permanentes da classe.
        // Se tem-se um dataset com milhões ou bilhões de linhas, cada linha virará um objeto.
        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
        std::string username;


        //
        static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2) {
            return e1.timestamp < e2.timestamp;
        };

        //
        static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2) {
            return e1.price < e2.price;
        };

        //
        static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2) {
            return e1.price > e2.price;
        };
      
};


/*
---------------------------------------------
RESUMO COMPLETO DE UMA CLASSE COMO EXEMPLO.
---------------------------------------------

class NomeDaClasse {
public:
    // 1. Construtor (mesmo nome da classe, É OPCIONAL)
    NomeDaClasse(tipo1 param1, tipo2 param2);

    // 2. Outras funções (métodos)
    tipo_retorno funcao1();
    void funcao2(tipo x);

    // 3. Variáveis (atributos)
    tipo1 atributo1;
    tipo2 atributo2;
};

---------------------------------------------
*/