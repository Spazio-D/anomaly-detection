#include "main.h"

std::vector<std::vector<double>> covarianceValue(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWindow, std::map<std::string, double> &averages){

    std::vector<std::vector<double>> covariances(sensors.size(), std::vector<double>(sensors.size()));
    std::string sensor1;
    std::string sensor2;
    int numberOfValue;
    double totalSum;

    for(size_t i = 0; i<sensors.size(); i++){

        sensor1 = sensors[i];

        for(size_t j = i; j<sensors.size(); j++){

            sensor2 = sensors[j];
            numberOfValue = 0;
            totalSum = 0;

            for(size_t k = 0; k<W; k++){

                if(dataWindow[sensor1][k].value == "NULL" || dataWindow[sensor2][k].value == "NULL"){
                    continue;
                }

                totalSum += (std::stod(dataWindow[sensor1][k].value) - averages[sensor1]) * (std::stod(dataWindow[sensor2][k].value) - averages[sensor2]);
                numberOfValue++;

            }

            if(numberOfValue == 0){

                covariances[i][j] = std::nan("");
                std::cout << sensor1 + " " + sensor2 + " NaN" << std::endl;

            }else{

                covariances[i][j] = totalSum/(numberOfValue - 1);
                std::cout << sensor1 + " " + sensor2 + " " << totalSum/(numberOfValue - 1) << std::endl;
                
            }

        }

    }
        
    return covariances;
}