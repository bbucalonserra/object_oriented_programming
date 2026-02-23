# C++


g++ main.cpp -o program

./programa


## Input and Output 
<< "Insertion operator, fluxos de saída"

>> "Extractor operator, fluxos de entrada"

std::endl "Flush out the buffers and go to next line"


namespace é para reduzir a quantidade de variaveis iguais. 
"std" + :: é o namespace standard mais "::".


* cout = console
* cin = input (keyboard como default), ele sempre ignora whitespaces.
* **O std::endl limpa o buffer e empurra para o monitor.**

* Buffer é uma regiao temporaria na memoria RAM. Em vez de o computador escrever no monitor ou no disco toda vez que o se chama um comando (o que seria extremamente lento), ele acumula os dados nesse "reservatório". Exemplo:
    - Quando o se usa std::cout << "Algo", o C++ não manda isso imediatamente para o monitor. Ele coloca em um buffer de saída. Depois, ele é "flushed" (descarregado) para: 
        1. (caso seja para o monitor no caso do cout) O Buffer de Memória da Placa de Vídeo (VRAM), sistema operacional e o driver de vídeo coordenam essa transferência e lá os caracteres são transformados em pixels pelo hardware da GPU e exibidos nos LEDs do seu monitor.
        2. (caso seja gravar um arquivo, ssd ou hd) O dado sai do seu buffer na RAM e vai para o Controlador de Disco, percorre os cabos (ou trilhas da placa-mãe, como no caso do NVMe) até chegar ao chip de memória do SSD, e o controlador do SSD então organiza esses elétrons para ficarem "presos" nas células de memória de forma permanente.
        3. (caso seja som) O dado sai do seu buffer de áudio na RAM e vai para o Buffer do Chip de Áudio (DAC - Digital to Analog Converter), se preenche um array de float na RAM e, no momento do flush, esses números são enviados para a placa de som, e o chip converte esses números em voltagem elétrica variável, que faz o ímã da sua caixa de som ou fone de ouvido vibrar, criando o som.
    - Existe porque comunicar-se com o hardware (I/O) é milhares de vezes mais lento que mexer na RAM. Agrupar os dados e enviá-los de uma vez economiza muitos ciclos da CPU.

* Flush é o ato de forçar o esvaziamento desse buffer, enviando todo o conteúdo acumulado diretamente para o destino final (o monitor ou um arquivo). O "Flush" acontece automaticamente em algumas situações:
    - Quando o buffer fica cheio.
    - Quando o programa termina com sucesso.
    - Quando o senhor solicita explicitamente no código.

Quando fazemos:

```cpp
#include <iostream>

int main() {

    int num1;
    int num2;

    std::cout << "Enter an 1 integer: " << std::endl;

    std::cin >> num1;

    std::cout << "Enter an 2 integer: " << std::endl;

    std::cin >> num2;

    std::cout << "You entered: " << num1 << " and " << num2 << std::endl;
}
```

E ao digitar 100 200 tudo no num1, ele ja vai printar direto os dois sem precisar colocar o input 2, porque tem um espaço entre eles entao ele ja armazena o 200 no num2, porque ambos ficam armazenados no buffer.

## Variables
Ao fazer:
int age;
age = 21;

Estamos fazendo uma abstração para um local de memória que estamos armazenando o valor 21.
Tipos:
* int (inteiro)
* double (float)
* string (texto)
* Etc.

Ao fazermos int age, estamos selecionando um espaço na memória (abstraindo com o nome age), por exemplo:
Escolhemos age, e vai apenas um inteiro. Ele fica alocado na memoria no local 2000, e este local 2000 são varios binarios (0s e 1s).


Formas de inicializar variáveis:
* int age; (nao inicializada)
* int age = 21; (C-like inicialization)
* int age (21); (Constructor initialization)
* int age {21}; (C++11 list initialization syntax)

NOTE: podemos adicionar const + int age para, por exemplo, nao mudar o valor de uma variável ("constant"). Idealmente NAO SE USA EM MODERN C++.

Primitive data types:

Strings:
* char = One byte, at least 8 bits
* char16_t = At least 16 bits.
* char32_t = At least 32 bits
* wchar_t = Can represent the largest availabe character  set.

Integer types (signed):
* signed short int = At least 16 bits.
* signed int = At least 16 bits.
* signed long int = At least 32 bits.
* signed long long int = At least 64 bits.

Integer types (unsigned). Unsigned é quando a máquina reserva o primeiro bit para dizer se é positivo ou negativo, o unsigned só pode ser positivo porque não há esta reserva
Como "ganha-se" um bit extra pra colocar o valor, o tamanho aumenta o dobro (* 2)
Ex: short com sinal vai de -32.768 a 32.767, um unsigned short vai de 0 a 65.535 :
* unsigned short int = At least 16 bits.
* unsigned int = At least 16 bits.
* unsigned long int = At least 32 bits.
* unsigned long long int = At least 64 bits.

Floating-point types:
* float = 7 decimal digits precision.
* double = No less than float / 15 decimal digits precision.
* long double = No less than double / 19 decimal digits precision.

Bool:
* bool = 8 bits (or true or false in keywords)

Podemos usar "#include <climits>" e sizeof(variavel) para saber o tamanho que cada
variavel cabe, o output é o valor em bytes.
Podemus usar também:
    * CHAR_MIN
    * INT_MIN
    * SHRT_MIN
    * LONG_MIN
    * LL_MIN

Para saber os "ranges" de cada.

## Arrays
Imutáveis.
```cpp

int test_score [5] = {100, 95, 99, 87, 88};
```
O 5 entre [] é o tamanho da array. Se nao colocar ele é decidido com base no tamanho da array.

O cout de test_score será "0x46ec3ffcb0", que é o endereço de memória hexadecimal onde o numero 100 está, é o "ponto de partida". Este score se traduzido do hexadecimal, vira "30,78,34,36,65,63,33,66,66,63,62,30".

Podemos fazer também multidimentional arrays:

```cpp
int test_score [3][4] {
    {0, 4, 3, 5},
    {2, 3, 3, 5},
    {1, 4, 4, 5}
};
```

No caso a cima, temos 3 LINHAS com 4 COLUNAS.

# Vectors
Diferente de arrays, nao temos limite de espaço. Arrays tem espaços pre definidos, diferentemente de vectors. Um vector é uma dynamic array.
Vectors sao objetos, entao precisamos "#include <vector>".

```cpp
std::vector <char> vowels;
std::vector <int> test_scores;
```

Podemos declarar também o tamanho:
```cpp
std::vector <char> vowels (10);
std::vector <int> test_scores (5);
```

Podemos escolher a inicialização dos valores, por exemplo:
```cpp
std::vector <int> numbers (10, 80);
```
No exemplo criamos um vector com 10 espaços todos com valor de 80. Para pegar algum indice usamos igual array com [].

**PARA ADICIONAR VALORES NUM VECTOR, USAMOS .push_back(valor)**:
```cpp
std::vector <int> test_scores = {100, 95, 99};

test_scores.push_back(90);
```

NOTE: PARA VECTOR, SEMPRE USAMOS "()" PARA TAMANHO E "{}" PARA OS VALORES QUE IRÃO DENTRO.

Vector tem um método .size() para saber o tamanho dele.

# Switch
Exemplo de switch:
```cpp
char letter_grade;

std::cout <<"Enter letter you expect as grade: " << std::endl;

std::cin >> letter_grade;

switch (letter_grade) {
    case 'a': 
    case 'A':
        std::cout << "You need a 90 or above, study harder" << std::endl;
        break;

    case 'b': 
    case 'B':
        std::cout << "You need a 80-89 or above, study harder" << std::endl;
        break;

    case 'c': 
    case 'C':
        std::cout << "You need a 70-79 or above, study harder" << std::endl;
        break;

    default:
        std::cout << "NOT VALID" << std::endl;
        break;
}
```

Dentro de switch nao se coloca condição, apenas variável.

## Loop
```cpp
for (inicialization; condition; increment) {
    xxxx
}
```

Range based loop é uma forma moderna de escrever o o loop:
```cpp
scores = {10, 20, 30}

for (auto score : scores) {
    std::cout << score;
}
```

Basicamente, o **auto** identifica sozinho o tipo de dados presentes na lista (int, float, string).

**Score** é o nome dado para o item atual, a cada volta no loop essa variavel recebe o valor do proximo item da lista.

**:** significa "dentro de" ou "pertencente a".

**scores** é a lista original (o array ou vector).

No exemplo: 
* 1ª volta: O programa pega o 10, coloca dentro de score e imprime.
* 2ª volta: O programa pega o 20, coloca dentro de score e imprime.
* 3ª volta: O programa pega o 30, coloca dentro de score e imprime.

(Em python já é assim).

## While

```cpp
while (expression){
    statement;
}
```

# Do while
Do é um loop, enquanto uma condição.

```cpp
int number;
do {
    cout << "xxx" >>
} while (selection == 'Y')
```

## Continue e Break
Usados no meio de um loop para continuar ou parar.

## Strings
Para usar strings devemos:
#include <string>

Além disso, podemos:

* string s1;
* string s2 {"Bruno"}; -> Bruno
* string s3 {s2}; -> Bruno
* string s4 {"Bruno", 3}; -> Bru
* string s5 {s3, 0, 2;} -> Br
* string s6 (3, 'X'); -> XXX

Só pomos concatenar c style strings, nao literal strings (ou seja so as criadas usando string, nao apenas com "".)

Temos:
* s1.substr(0, 4); -> retorna indice de 0 a 3 (4 exclusivo)
* s1.find("This"); -> Em "This is great", retorna 0 porque está no indice 0 antes do espaço (vazio na memória).
* s1.length() -> tamanho

# Function
Parafunções que nao retornam nada, temos que usar "void" antes da definição ("void sum").
Caso retorne, precisamos dizer o tipo do dado retornado ("int sum_numbers").

Overloading functions -> Passamos o mesmo nome da função com parametros diferentes, ao chama-la,
o programa vai escolher a adequada com base nos parametros passados.

NOTE: size_t é um tipo de dado, assim como int, float ou char. No entanto, para ser tecnicamente preciso em C++, ele é o que chamamos de um alias (um apelido) para um tipo de número inteiro sem sinal (unsigned integer).
O compilador define o size_t de forma que ele seja sempre grande o suficiente para conter o tamanho do maior objeto que o seu computador consegue criar na memória.
    * Em sistemas de 64 bits: O size_t é geralmente um unsigned long long (8 bytes).
    * Em sistemas de 32 bits: O size_t é geralmente um unsigned int (4 bytes).

Exemplo:
```cpp
void zero_array(int numbers[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        numbers[i] = 0;
    }
}
```

NOTE: O operador "&" tem dois significados:
    * & no Tipo (Referência) -> A declaração de um parâmetro de função ou de uma variável, o senhor está criando uma Referência. Isso significa que o senhor não está criando uma cópia do dado, mas sim um "apelido" para a variável original. PODE SER USADO COM V.CLEAR() PARA RETIRAR A VAR. Exemplo:
```cpp
void dobrar(int &numero) { // O & indica que 'numero' é uma referência
    numero = numero * 2;   // Altera diretamente a variável original
}

int main() {
    int x = 10;
    dobrar(x); 
    // Agora x vale 20! Em Python, isso não aconteceria com inteiros.
}
```

    * & antes da Variável (Operador de Endereço) -> Antes de uma variável que já existe em uma expressão, o senhor está pedindo ao C++ o endereço de memória onde aquela variável está guardada.

```cpp
int main() {
    int valor = 42;
    std::cout << valor << std::endl;  // Imprime 42
    std::cout << &valor << std::endl; // Imprime algo como 0x7ffd5e (o endereço na RAM)
}
```


Inline functions são mais rapidas porque ao inves de ir a outro arquivo fazer o processo e retornar, ele copia e cola a função e já executa. Ao colocar a palavra inline antes da função para dizer ao compilador: "Não perca tempo pulando para outro lugar da memória; apenas copie o código desta função aqui dentro".

```cpp
inline int add_numbers(int a, int b) {
    return a + b
}
```

## SCOPE RULES
Primeiro sempre olhamos as variáveis dentro do escopo (entre "{}"), depois olhamos as globais. Sempre de dentro pra fora.


## Pointeiros

```cpp
int main() {

    // Pointer. Variável que aponta para um inteiro, ela tem um endereço do 
    // outro inteiro.
    int *int_ptr = nullptr;

    // Agora, para alocar do heap, a storage do integer vai ser alocada
    // no heap e seu endereço sera alocado no int_ptr.
    int_ptr = new int;

    *int_ptr = 100;

    // ACESSO: Imprime o valor que está no heap
    std::cout << "Valor na stack: " << &int_ptr << std::endl;
    std::cout << "Valor no heap: " << *int_ptr << std::endl;
    std::cout << "Endereco no heap: " << int_ptr << std::endl;

    // Libera a storage.
    delete int_ptr;

    /*
        int_ptr é uma variável NA STACK.
        &int_ptr é o endereço desta variavel NA STACK.
        A variável int_ptr armazena um endereço no heap (aponta para este endereço no heap).
        O que está armazenado neste endereço no heap é *int_ptr.

    */

    return 0;
}

```

No exemplo abaixo, o operador "++" tem prioridade. Logo, no while primeiro adiciona 1 no endereço de memoria (vai pra proxima) depois printa o valor.

Quando você incrementa um ponteiro, o C++ não soma simplesmente 1 unidade de memória (1 byte); ele soma 1 unidade do tipo de dado que o ponteiro aponta.

O ponteiro é de um int. Na maioria das arquiteturas modernas (como a que você está usando), um int ocupa 4 bytes. Se o endereço atual de scores_ptr for 1000, quando você faz ++:
    * O C++ pensa: "Esse ponteiro aponta para um int. O próximo int começa 4 bytes depois."
    * O endereço passa a ser 1004.

Se fosse um ponteiro de double (que ocupa 8 bytes), o ++ pularia do endereço 1000 para o 1008.


```cpp
int main() {

    int scores[] = {100, 95, 89, -1};

    int *scores_ptr = scores;

    while (*scores_ptr != -1) {
        std::cout << *scores_ptr << std::endl;
        *scores_ptr++;
    }

    return 0;
}
```