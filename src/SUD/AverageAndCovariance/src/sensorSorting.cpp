#include "main.h"

bool sensorSorting(const std::string &str1, const std::string &str2) {

    // Estazione dei numeri dai nomi dei sensori
    int num1 = std::stoi(str1.substr(3));
    int num2 = std::stoi(str2.substr(3));

    return num1 < num2;
}