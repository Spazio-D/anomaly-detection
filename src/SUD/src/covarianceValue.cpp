#include "main.h"

std::map<std::string, std::vector<double>> covarianceValue(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWindowNoNull, std::map<std::string, double> &averages){

    std::map<std::string, std::vector<double>> covariances;
    std::string sensor1;
    std::string sensor2;

    for(size_t i = 0; i<sensors.size() - 1; i++){
        
        sensor1 = sensors[i];
        auto check1 = dataWindowNoNull.find(sensor1);

        if(check1 == dataWindowNoNull.end()){
            continue;
        }

        for(size_t j = i + 1; j<sensors.size(); j++){
            
            sensor2 = sensors[j];
            auto check2 = dataWindowNoNull.find(sensor2);

            if(check2 == dataWindowNoNull.end()){
                covariances[sensor1].push_back(std::numeric_limits<double>::max());
                continue;
            }

            int numberOfValue = 0;
            double totalSum = 0;
            // Se non sbaglio "k<dataWindowNoNull[sensor1]" potrà essere sostituita con la grandezza della finestra W
            for(size_t k = 0; k<dataWindowNoNull[sensor1].size(); k++){

                if(dataWindowNoNull[sensor1][k].value == "NULL" || dataWindowNoNull[sensor2][k].value == "NULL"){
                    continue;
                }

                /*Forse dovrei fare check se i sampleTime dei due sensori siano uguali, ma in verità so che le strutture datai sono
                sortate e inoltre devo pensare come se i dati mi arrivano sequenzialmente in real time, quindi dovrebbero essere sortati
                per forza di cose, no? boh so stanco so le 2.13 me se stanno a squaglià gli occhi.*/

                totalSum += (std::stod(dataWindowNoNull[sensor1][k].value) - averages[sensor1]) * (std::stod(dataWindowNoNull[sensor2][k].value) - averages[sensor2]);
            }

            covariances[sensor1].push_back(totalSum/(numberOfValue - 1));
            std::cout << totalSum/(numberOfValue - 1) << std::endl;

        }
    }

    return covariances;
}