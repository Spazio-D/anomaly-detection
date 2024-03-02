#include "main.h"

std::map<std::string, std::vector<Data>> createDataWindow(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataNoNull, int wStart, int wEnd){

    std::map<std::string, std::vector<Data>> dataWindowNoNull;

    for(std::string sensor : sensors){

        if(dataNoNull.find(sensor) != dataNoNull.end()){
            
            for(Data data : dataNoNull[sensor]){

                if(std::stoi(data.sampleTime) >= wStart && std::stod(data.sampleTime) <= wEnd ){
                    dataWindowNoNull[sensor].push_back(data);
                }
            }
        }
    }

    return dataWindowNoNull;
}