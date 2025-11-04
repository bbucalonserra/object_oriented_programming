#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// A função vai devolver um vetor em string.
// É igual fazer int funcao, so que o int seria o vector string e a funcao o 'tokenize'.

// ESSA FUNÇÃO SERVE para dar append em strings em um vetor, tirando seus separadores.
std::vector<std::string> tokenise(std::string csvLine, char separator) {

    // "std::vector<std::string>" - define o tipo, vetor listas.
    // tokens - nome da variável
    // Quando faço o código abaixo, eu estou instanciando essa classe no objeto tokens.
    // É IGUAL AO INT X, SO QUE O TOKENS ABAIXO NAO É UMA VARIÁVEL E SIM UM OBJETO.
    std::vector<std::string> tokens;
    
    
    std::string token;

    // Signed significa que o tipo permite valores negativos e positivos.
    signed int start, end;

    // find_first_not_of é da classe string, que procure primeiro caractere que nao precente ao conjunto especificado.
    // find_first_not_of(caracteres, pos_inicial[por onde começa a busca]);
    // Primeiro caracter que nao seja 'separator'
    // Exemplo: bruno, sofia, brenda,... aqui seria o 'b', output: 0.
    // OUTPUT É NUMERICO. 
    start = csvLine.find_first_not_of(separator, 0);

    // Do While loop.
    do {
        // Busca o indice do primeiro separador, começando no start (ou seja no primeiro nao separador).
        end = csvLine.find_first_of(separator, start);

        // Se start for tamanho da linha ou start for igual ao fim, para.
        if (start == csvLine.length() || start == end) break;

        // Se start for maior que 0, subtrai.
        // O subsctract é substr(pos_inicial[posicao / indice do caractee], comprimento[quantos caracteres extrair])
        // Logo, vai pegar os caracteres entre separadores.
        if (end >= 0) token = csvLine.substr(start, end - start);

        // Caso seja <= 0, ou seja, nao tem mais separador depois de start, vc esta lendo o ultimo campo.
        // Logo, pega o resto da string ate o fim.
        else token = csvLine.substr(start, csvLine.length() - start);

        // O push_back é um método da classe vector.
        tokens.push_back(token);

        start = end + 1;

    // Quando funções/metodos nao encontram nada, eles retornam "std::string::npos".
    } while (end != std::string::npos);


    return tokens;
}

int main() {

    // Crio um objeto chamado tokens (o da função so existe no escopo da função).
    std::vector<std::string> tokens;

    // std::ifstream é a classe, csvFile é o objeto.
    // O "{}"" significa que estamos chamando o CONSTRUCTOR
    std::ifstream csvFile{"Fireball_And_Bolide_Reports_20241128.csv"};

    // Criando um objeto chamado line para o string.
    std::string line;

    if (csvFile.is_open()) {
        // Verificar se o arquivo está aberto. Ou seja se csvFile.is_open() = true.
        std::cout << "File opened." << std::endl;


        // Aqui, vamos usar o método getline para ler o objeto (instancia da classe) csvFile, o segundo o mesmo porém ONDE O TEXTO SERÁ GUARDADO.
        // Em suma, estamos usando o método getline para ler o objeto de csv,  
        // Se chama-se duas vezes, ele automaticamente le a proxima linha.
        // Logo, usamos um loop:

        // O while abaixo seria “Enquanto conseguir ler uma linha do arquivo, execute o bloco.”.
        while(std::getline(csvFile, line)) {
            // Printando para ler o objeto line.
            std::cout << "Read line: " << line << std::endl;

            tokens = tokenise(line, ',');

            // Eu preciso fazer isso porque a função retorna um vector (pq no começo dela, esta: std::vector<std::string>).
            // Logo, eu preciso "igualar" a variável que será o output, no caso é tokens.
            /* OU:
                for (std::string& t : tokens) {
                    std::cout << s << std::endl;
                }  
            */

            // Checando se a linha tem 5 informações.
            if (tokens.size() != 5) {
                std::cout << "Bad line" << std::endl;
                continue;
            }

            try {
                // Caso tenha:
                // Stod converte uma std::string (texto) em um número do tipo double.
                // Stod = String TO Double.
                float price = std::stod(tokens[3]);
                float amount = std::stod(tokens[4]);
            } catch (std::exception & e){
                std::cout << "Bad float" << tokens[3] << std::endl;
                std::cout << "Bad float" << tokens[4] << std::endl;
                continue; // Aqui para ver poderiamos dar um break para parar o print.
            }

            // for (auto i = 0; i<tokens.size(); i++) {
            //     std::cout << tokens[i] << std::endl;
            // }
        }

        // Fechar.
        csvFile.close();
    }

    else {
        std::cout << "Could not open file." << std::endl;
    }

    return 0;
}