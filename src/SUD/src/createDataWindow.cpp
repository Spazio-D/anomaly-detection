#include "main.h"

std::map<std::string, std::vector<Data>> createDataWindow(std::map<std::string, std::vector<Data>> &dataNoNull, int wStart, int wEnd){

    std::map<std::string, std::vector<Data>> dataWindowNoNull;

    for(auto element : dataNoNull){
            
        for(Data data : dataNoNull[element.first]){

            if(std::stoi(data.sampleTime) >= wStart && std::stod(data.sampleTime) <= wEnd ){
                dataWindowNoNull[element.first].push_back(data);
            }
        }
    }

    return dataWindowNoNull;
}