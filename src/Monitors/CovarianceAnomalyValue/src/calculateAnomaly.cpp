#include "main.h"

std::vector<std::vector<std::vector<AnomalyCovariance>>> calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, std::vector<std::vector<std::vector<Covariance>>> &covariances){

    std::string valS1;
    std::string valS2;
    double averageS1;
    double averageS2;
    double anomalyValue;
    Covariance covariance;
    std::vector<std::vector<std::vector<AnomalyCovariance>>> covarianceAnomalyVector{};

    for(size_t i = 0; i < covariances.size(); i++){

        covarianceAnomalyVector.emplace_back();

        for(size_t j = 0; j < covariances[i].size(); j++){

            covarianceAnomalyVector[i].emplace_back();

            for(size_t k = 0; k < covariances[i][j].size(); k++){

                covariance = covariances[i][j][k];
                covarianceAnomalyVector[i][j].emplace_back();
                covarianceAnomalyVector[i][j][k].sensorID1 = "SAC" + std::to_string(i);
                covarianceAnomalyVector[i][j][k].sensorID2 = "SAC" + std::to_string(j);
                covarianceAnomalyVector[i][j][k].sampleTime = k;

                valS1 = dataVector["SAC" + std::to_string(i)][covariance.lastSampleTime].value;
                valS2 = dataVector["SAC" + std::to_string(j)][covariance.lastSampleTime].value;
                if(valS1 == "" || valS2 == ""){
                    covarianceAnomalyVector[i][j][k].value = std::nan("");
                    std::cout << covarianceAnomalyVector[i][j][k].sensorID1 << " " << covarianceAnomalyVector[i][j][k].sensorID2 << " " << covarianceAnomalyVector[i][j][k].value << " " << covarianceAnomalyVector[i][j][k].sampleTime << std::endl;

                    continue;
                }

                averageS1 = averages["SAC" + std::to_string(i)][k].value;
                averageS2 = averages["SAC" + std::to_string(j)][k].value;
                anomalyValue = ((std::stod(valS1) - averageS1) * (std::stod(valS2) - averageS2)) / covariance.value;
                covarianceAnomalyVector[i][j][k].value = anomalyValue;
              
                //std::cout << covarianceAnomalyVector[i][j][k].sensorID1 << " " << covarianceAnomalyVector[i][j][k].sensorID2 << " " << covarianceAnomalyVector[i][j][k].value << " " << covarianceAnomalyVector[i][j][k].sampleTime << std::endl;

            }
        }
    }

    return covarianceAnomalyVector;

}
