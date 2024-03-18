#include "main.h"

void detectAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages){

    // Calcolo della grandezza della finestra temporale e scorrimento sui dati dei sensori
    std::string key = averages.begin()->first;
    int windowSize = averages[key][0].lastSampleTime + 1;
    for(auto sensor : dataVector){
        for(size_t i = 0; i<sensor.second.size() - windowSize + 1; i++){

            int numberOfValues = 0;
            Average average = averages[sensor.first][i];
            double deviation = 0;

            // Scorrimento sulla finestra temporale e calcolo della deviazione standard
            double sensorValue;
            for(int j = i; j < windowSize; j++){

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

            double standardDeviation = std::sqrt(deviation / numberOfValues);

            if(average.value - (theta * standardDeviation) < sensorValue && average.value + (theta * standardDeviation) > sensorValue){
                dataVector[sensor.first][average.lastSampleTime].isAverageAnomaly = false;
            }else{
                dataVector[sensor.first][average.lastSampleTime].isAverageAnomaly = true;
            }

        }
    }

    return;
}


