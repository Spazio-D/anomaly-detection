#include "main.h"

std::map<std::string, double> averageValue(std::map<std::string, std::vector<Data>> &dataWindowNoNull){

    std::map<std::string, double> averages;

    for(auto element : dataWindowNoNull){

        double totalValue = 0;
        int numberOfValue = dataWindowNoNull[element.first].size();

        for(Data data : dataWindowNoNull[element.first]){
            totalValue += std::stod(data.value);
        }

        double average = totalValue/numberOfValue;
        averages[element.first] = average;

        std::cout << element.first << "\t" << average << std::endl;        
    }

    return averages;
}