#include "main.h"

void calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages){

    std::string key = averages.begin()->first;
    int windowSize = averages[key][0].lastSampleTime + 1;
    double standardDeviation;
    double deviation;
    Average average;
    double sensorValue;
    std::string check;
    int numberOfValues;

    for(auto sensor : dataVector){
        for(size_t i = 0; i<sensor.second.size() - windowSize + 1; i++){

            numberOfValues = 0;
            average = averages[sensor.first][i];
            deviation = 0;

            for(int j = i; j < windowSize; j++){

                check = sensor.second[j].value;
                if(check == ""){
                    continue;
                }
                
                sensorValue = std::stod(check);
                deviation += std::pow(sensorValue - average.value, 2);
                numberOfValues++;

            }

            if(numberOfValues == 0){
                //std::cout << "sensorID: " << averages[sensor.first][i].sensorID << " firstSampleTime: " << averages[sensor.first][i].firstSampleTime << " isAnomaly: " << averages[sensor.first][i].isAnomaly << std::endl;
                continue;
            }

            standardDeviation = std::sqrt(deviation / numberOfValues);

            sensor.second[average.lastSampleTime].averageAnomalyValue = (std::stod(sensor.second[average.lastSampleTime].value) - average.value) /standardDeviation;

        }
    }

    return;
}


