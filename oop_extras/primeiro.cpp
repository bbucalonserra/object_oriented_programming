#include <iostream>

class carro {
    private:
        int ano; //esse é o que vai no "this".
        float valor; //esse é o que vai no "this".
        float km; //esse é o que vai no "this".


    public:

        ///////////////// CONSTRUTOR /////////////////
        // TEM QUE TER O MESMO NOME DA CLASSE!! 
        // "Ano vai receber a, valor vai receber v, km vai receber k."
        // Isso faz com que eu possa nao precisar chamar a função set. Por exemplo: carro palio (1995, 10000, 15000)
        carro(int a=0, float v=-1, float k=-1)  {
        ano = a;
        valor = v;
        km = k;
        }

        ///////////////// SET E GET /////////////////
        // Set.
        void setAno(int a) {
            // "O ano desta classe vai receber o ano do que o usuário passou."
            this->ano = a;
        }

        void setValor(float v) {
            this->valor = v;
        }

        void setKm(float k) {
            this->km = k;
        }

        // Get.
        int getAno() {
            return ano;
        }

        float getValor() {
            return valor;
        }

        float getKm() {
            return km;
        }

};



int main() {

    carro palio (1995, 10000, 15000);

    // palio.setAno(1995);
    // palio.setValor(10000);
    // palio.setKm(150000);

    std::cout << "Palio: \n";
    std::cout << "  Ano: " << palio.getAno() << std::endl;
    std::cout << "  Valor: " << palio.getValor() << std::endl;
    std::cout << "  Km: " << palio.getKm() << std::endl;


    
    carro celta (2002, 15460, 90000);

    // celta.setAno(2002);
    // celta.setValor(15460);
    // celta.setKm(90000);

    std::cout << "Celta: \n";
    std::cout << "  Ano: " << celta.getAno() << std::endl;
    std::cout << "  Valor: " << celta.getValor() << std::endl;
    std::cout << "  Km: " << celta.getKm() << std::endl;

    return 0;
    
}