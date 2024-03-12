#include "main.h"

bool sensorSorting(const std::string &str1, const std::string &str2) {
    // Estrai le cifre dalla stringa
    int num1 = std::stoi(str1.substr(3));  // Salta i primi tre caratteri "SAC"
    int num2 = std::stoi(str2.substr(3));

    // Confronta le cifre
    return num1 < num2;
}