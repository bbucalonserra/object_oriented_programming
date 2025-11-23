#include "OrderBookEntry.h"

// Aqui, estamos dizendo: “Quando alguém quiser criar um OrderBookEntry, ele tem que me entregar 5 valores.”
// Literalmente apenas parâmetros.
// Abaixo é a definição do método fora da classe.
// O motivo de ter que escrever duas vezes double _price etc. é que o compilador de C++ trata declaração (.h) e definição (cpp) como instruções independentes.
// Chamamos de assinatura o que vem do .h.
OrderBookEntry::OrderBookEntry( double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType,
                        std::string _username)


// Aqui já são as variáveis internas da classe. São os espaços de memória permanentes da classe.
// Se tem-se um dataset com milhões ou bilhões de linhas, cada linha virará um objeto.
// Optou-se por usar ":", que é uma lista, porque assim as variáveis já são criadas com o valor certo (ou seja, com o qual for passado).
// Se fosse diferente, primeiro ele criaria os atributos vazios (price, amount, etc.) e depois sobrescreve os valores recebidos.
// Não muda nada, apenas é mais performático.
: price(_price), 
  amount(_amount), 
  timestamp(_timestamp),
  product(_product), 
  orderType(_orderType),
  username(_username)

// Abaixo é o bloco de código que será executado quando a função for chamada.
// Quando não há instrução (abaixo, vazio), ele só inicializa com aquelas variáveis.
{
    
}

// OrderBookType é o tipo da função. Aqui ele sabe que é enum pois foi declarado no .h.
// Voce declara um objeto chamado OrderBookType que é um enum class.
// A estrutura é: <tipo_de_retorno> <escopo>::<nome_da_função>(<parâmetros>)
// AQUI NAO CRIOU OBJETO porque a função é STATIC.
OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
  if (s == "ask")
  {
    return OrderBookType::ask;
  }
  if (s == "bid")
  {
    return OrderBookType::bid;
  }
  return OrderBookType::unknown;
}
 
