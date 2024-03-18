#include "main.h"

void calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages){

    // Calcolo della grandezza della finestra temporale e scorrimento sul vettore di dati
    std::string key = averages.begin()->first;
    int windowSize = averages[key][0].lastSampleTime + 1;
    for(auto sensor : dataVector){
        for(size_t i = 0; i<sensor.second.size() - windowSize + 1; i++){

            int numberOfValues = 0;
            Average average = averages[sensor.first][i];
            double deviation = 0;

            // Scorrimento della finestra temporale e calcolo della deviazione standard
            for(int j = i; j < windowSize; j++){

                std::string sensorValue = sensor.second[j].value;
                if(sensorValue == ""){
                    continue;
                }
                
                deviation += std::pow(std::stod(sensorValue) - average.value, 2);
                numberOfValues++;

            }

            if(numberOfValues == 0){
                continue;
            }

            double standardDeviation = std::sqrt(deviation / numberOfValues);
            sensor.second[average.lastSampleTime].averageAnomalyValue = (std::stod(sensor.second[average.lastSampleTime].value) - average.value) /standardDeviation;
        }
    }

    return;
}


