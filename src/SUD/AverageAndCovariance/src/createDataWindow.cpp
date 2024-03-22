#include "main.h"

std::map<std::string, std::vector<Data>> createDataWindow(std::map<std::string, std::vector<Data>> &dataVector, int wStart, int wEnd){

    std::map<std::string, std::vector<Data>> dataWindow;

    // Scorrimento del vettore di dati dei sensori
    for(auto element : dataVector){
        
        // Scorrimento dei dati del sensore e creazione della finestra temporale
        for(Data data : element.second){
            if(std::stoi(data.sampleTime) >= wStart && std::stod(data.sampleTime) <= wEnd ){
                dataWindow[element.first].push_back(data);
            }
        }
    }

    return dataWindow;
}