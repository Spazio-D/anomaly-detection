#include "main.h"

std::map<std::string, std::vector<Data>> createDataWindow(std::map<std::string, std::vector<Data>> &dataVector, int wStart, int wEnd){

    std::map<std::string, std::vector<Data>> dataWindow;

    for(auto element : dataVector){
            
        for(Data data : dataVector[element.first]){

            if(std::stoi(data.sampleTime) >= wStart && std::stod(data.sampleTime) <= wEnd ){
                dataWindow[element.first].push_back(data);
            }
        }
    }

    return dataWindow;
}