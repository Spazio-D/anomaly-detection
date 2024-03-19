#include "main.h"

void calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages){

    // Calcolo della grandezza della finestra temporale e scorrimento sul vettore di dati
    std::string key = averages.begin()->first;
    int windowSize = averages[key][0].lastSampleTime + 1;
    for(auto sensor : dataVector){
        for(size_t i = 0; i < sensor.second.size() - windowSize + 1; i++){

            int numberOfValues = 0;
            Average average = averages[sensor.first][i];
            double deviation = 0;

            // Scorrimento della finestra temporale e calcolo della deviazione standard
            double sensorValue;
            for(int j = i; j < windowSize + i; j++){

                if(sensor.second[j].value == ""){
                    continue;
                }
                
                sensorValue = std::stod(sensor.second[j].value);
                deviation += std::pow(sensorValue - average.value, 2);
                numberOfValues++;

            }

            if(numberOfValues == 0){
                continue;
            }

            double standardDeviation;
            if(deviation == 0){
                standardDeviation = 0.000001;
            }else{
                standardDeviation = std::sqrt(deviation / numberOfValues);
            }

            dataVector[sensor.first][average.lastSampleTime].averageAnomalyValue = (std::stod(dataVector[sensor.first][average.lastSampleTime].value) - average.value) / standardDeviation;
        }
    }

    return;
}


