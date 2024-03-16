#include "main.h"

void detectAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages){

    std::string key = averages.begin()->first;
    int windowSize = averages[key][0].lastSampleTime + 1;
    double standardDeviation;
    double deviation;
    Average average;
    int numberOfValues;
    double sensorValue;
    
    for(auto sensor : dataVector){
        for(size_t i = 0; i<sensor.second.size() - windowSize + 1; i++){

            numberOfValues = 0;
            average = averages[sensor.first][i];
            deviation = 0;

            for(int j = i; j < windowSize; j++){

                if(sensor.second[j].value == ""){
                    continue;
                }
                sensorValue = std::stod(sensor.second[j].value);
                deviation += std::pow(sensorValue - average.value, 2);
                
                numberOfValues++;

            }

            if(numberOfValues == 0){
                ////std::cout << "sensorID: " << averages[sensor.first][i].sensorID << " firstSampleTime: " << averages[sensor.first][i].firstSampleTime << " isAnomaly: " << averages[sensor.first][i].isAnomaly << std::endl;
                continue;
            }

            standardDeviation = std::sqrt(deviation / numberOfValues);

            if(average.value - (theta * standardDeviation) < sensorValue && average.value + (theta * standardDeviation) > sensorValue){
                dataVector[sensor.first][average.lastSampleTime].isAverageAnomaly = false;
                //std::cout << "sensorID: " << averages[sensor.first][i].sensorID << " firstSampleTime: " << averages[sensor.first][i].firstSampleTime << " isAnomaly: " << averages[sensor.first][i].isAnomaly << std::endl;
            }else{
                dataVector[sensor.first][average.lastSampleTime].isAverageAnomaly = true;
                //std::cout << "sensorID: " << averages[sensor.first][i].sensorID << " firstSampleTime: " << averages[sensor.first][i].firstSampleTime << " isAnomaly: " << averages[sensor.first][i].isAnomaly << std::endl;
            }

        }
    }

    return;
}


