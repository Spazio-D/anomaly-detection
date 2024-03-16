#include "main.h"

std::map<std::string, double> averageValue(std::map<std::string, std::vector<Data>> &dataWindow){

    std::map<std::string, double> averages;

    // Scorrimento dei dati della finestra temporale
    for(auto element : dataWindow){

        // Calcolo della media con esclusione dei valori NULL
        double totalSum = 0;
        int numberOfValue = 0;
        for(Data data : element.second){
            if(data.value != "NULL"){
                totalSum += std::stod(data.value);
                numberOfValue++;
            }
        }

        // Salvataggio della media
        if(numberOfValue == 0){
            averages[element.first] = std::nan("");
        }else{
            averages[element.first] = totalSum/numberOfValue;
        }
        
    }

    return averages;
}