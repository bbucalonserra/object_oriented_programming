#include <iostream>
#include <vector>
#include <cctype>
#include <cstring>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

void double_data(int *int_ptr) {
    *int_ptr = 2 * *int_ptr ;
}


int main() {

    int value = 10;

    int *int_ptr = nullptr;

    std::cout << value << std::endl;

    int_ptr = &value;

    double_data(int_ptr);

    std::cout << value << std::endl;

    return 0;
}
