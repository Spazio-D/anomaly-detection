#include "main.h"

void detectAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages){

    // AL POSTO DI SAC0 METTERE IL PARAMETRO CONFIGURABILE DEL PRIMO SENSORE
    int windowSize = averages["SAC0"][0].lastSampleTime + 1;
    double standardDeviation;
    double devation;
    Average average;
    double sensorValue;
    std::string check;
    int numberOfValues;

    for(auto sensor : dataVector){
        for(size_t i = 0; i<sensor.second.size() - windowSize + 1; i++){

            numberOfValues = 0;
            average = averages[sensor.first][i];
            devation = 0;

            for(int j = i; j < windowSize; j++){

                check = sensor.second[j].value;
                if(check == ""){
                    continue;
                }
                
                devation += std::pow(std::stod(check) - average.value, 2);
                sensorValue = std::stod(check);
                numberOfValues++;

            }

            if(numberOfValues == 0){
                ////std::cout << "sensorID: " << averages[sensor.first][i].sensorID << " firstSampleTime: " << averages[sensor.first][i].firstSampleTime << " isAnomaly: " << averages[sensor.first][i].isAnomaly << std::endl;
                continue;
            }

            standardDeviation = std::sqrt(devation / numberOfValues);

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


