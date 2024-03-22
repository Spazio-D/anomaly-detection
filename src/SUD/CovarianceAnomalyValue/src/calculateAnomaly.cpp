#include "main.h"

std::vector<std::vector<std::vector<AnomalyCovariance>>> calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, std::vector<std::vector<std::vector<Covariance>>> &covariances){

    // Creazione matrice per le anomalie delle covarianze
    std::vector<std::vector<std::vector<AnomalyCovariance>>> covarianceAnomalyVector{};

    // Scorrimento della matrice di covarianze
    for(size_t i = 0; i < covariances.size(); i++){

        covarianceAnomalyVector.emplace_back();

        for(size_t j = 0; j < covariances[i].size(); j++){

            covarianceAnomalyVector[i].emplace_back();

            // Scorrimento della finestra temporale e popolazione della matrice delle anomalie delle covarianze
            for(size_t k = 0; k < covariances[i][j].size(); k++){

                covarianceAnomalyVector[i][j].emplace_back();
                covarianceAnomalyVector[i][j][k].sensorID1 = "SAC" + std::to_string(i);
                covarianceAnomalyVector[i][j][k].sensorID2 = "SAC" + std::to_string(j);
                covarianceAnomalyVector[i][j][k].sampleTime = k;

                // Lettura valore dei due sensori ad un determinato timeStamp
                Covariance covariance = covariances[i][j][k];
                std::string valS1 = dataVector["SAC" + std::to_string(i)][covariance.lastSampleTime].value;
                std::string valS2 = dataVector["SAC" + std::to_string(j)][covariance.lastSampleTime].value;
                if(valS1 == "" || valS2 == ""){
                    covarianceAnomalyVector[i][j][k].value = std::nan("");
                    continue;
                }

                // Lettura media dei due sensori di una determinata finestra, calcolo del valore dell'anomalia e salvataggio nella matrice
                double averageS1 = averages["SAC" + std::to_string(i)][k].value;
                double averageS2 = averages["SAC" + std::to_string(j)][k].value;
                double anomalyValue;
                if (covariance.value == 0){
                    anomalyValue = ((std::stod(valS1) - averageS1) * (std::stod(valS2) - averageS2)) / 0.000001;
                    covarianceAnomalyVector[i][j][k].value = anomalyValue;
                    continue; 
                }

                anomalyValue = ((std::stod(valS1) - averageS1) * (std::stod(valS2) - averageS2)) / covariance.value;
                covarianceAnomalyVector[i][j][k].value = anomalyValue;              
            }
        }
    }

    return covarianceAnomalyVector;
}
