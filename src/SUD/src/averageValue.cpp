#include "main.h"

std::map<std::string, double> averageValue(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWindowNoNull){

    std::map<std::string, double> averages;

    for(std::string sensor : sensors){

        if(dataWindowNoNull.find(sensor) != dataWindowNoNull.end()){

            double totalValue = 0;
            int numberOfValue = dataWindowNoNull[sensor].size();

            for(Data data : dataWindowNoNull[sensor]){
                totalValue += std::stod(data.value);
            }

            double average = totalValue/numberOfValue;
            averages[sensor] = average;

            std::cout << sensor << "\t" << average << std::endl;
        }
    }

    return averages;
}