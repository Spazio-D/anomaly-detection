#include "main.h"

std::map<std::string, double> averageValue(std::map<std::string, std::vector<Data>> &dataWindow){

    std::map<std::string, double> averages;
    double totalSum;
    int numberOfValue;
    double average;

    for(auto element : dataWindow){

        totalSum = 0;
        numberOfValue = 0;

        for(Data data : dataWindow[element.first]){

            if(data.value != "NULL"){
                totalSum += std::stod(data.value);
                numberOfValue++;
            }

        }

        if(numberOfValue == 0){

            averages[element.first] = std::nan("");
            std::cout << element.first << "\t" << "NaN" << std::endl;

        }else{

            average = totalSum/numberOfValue;
            averages[element.first] = average;
            std::cout << element.first << "\t" << average << std::endl;

        }
        
    }

    return averages;
}