#include "main.h"

std::vector<std::vector<double>> covarianceValue(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWindow, std::map<std::string, double> &averages){

    // Scorrimento dei sensori per la creazione della matrice di covarianza
    std::vector<std::vector<double>> covariances(sensors.size(), std::vector<double>(sensors.size()));
    for(size_t i = 0; i<sensors.size(); i++){

        std::string sensor1 = sensors[i];

        for(size_t j = 0; j<sensors.size(); j++){

            std::string sensor2 = sensors[j];
            int numberOfValue = 0;
            double totalSum = 0;

            // Scorrimento dei dati della finestra temporale e calcolo della covarianza
            for(size_t k = 0; k<WINDOW_SIZE; k++){

                if(dataWindow[sensor1][k].value == "NULL" || dataWindow[sensor2][k].value == "NULL"){
                    continue;
                }

                totalSum += (std::stod(dataWindow[sensor1][k].value) - averages[sensor1]) * (std::stod(dataWindow[sensor2][k].value) - averages[sensor2]);
                numberOfValue++;
            }

            // Salvataggio dei dati nella matrice di covarianza
            if(numberOfValue == 0){
                covariances[i][j] = std::nan("");
            }else{
                covariances[i][j] = totalSum/numberOfValue;                
            }

        }

    }
        
    return covariances;
}