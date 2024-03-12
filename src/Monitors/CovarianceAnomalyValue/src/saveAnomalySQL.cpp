#include "main.h"

bool saveAnomalySQL(std::vector<std::vector<std::vector<AnomalyCovariance>>> &covarianceAnomalyVector, PGconn *conn){
    
    PGresult *res;
    std::string query;
    std::string isAnomaly;

    for(size_t i = 0; i < covarianceAnomalyVector.size(); i++){
        for(size_t j = 0; j < covarianceAnomalyVector[i].size(); j++){
            for(size_t k = 0; k < covarianceAnomalyVector[i][j].size(); k++){
                AnomalyCovariance anomaly = covarianceAnomalyVector[i][j][k];
                std::string value = std::isnan(anomaly.value) ? "NULL" : std::to_string(anomaly.value);
                query = "INSERT INTO anomalyCovarianceTable (sensorID1, sensorID2, firstSampleTime, value) VALUES ('" + anomaly.sensorID1 + "', '" + anomaly.sensorID2 + "', " + std::to_string(k) + ", " + value + ")";
                res = PQexec(conn, query.c_str());
                if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                    std::cerr << "Errore durante l'esecuzione della query di inserimento anomalie di covarianza: " << PQresultErrorMessage(res) << std::endl;
                    PQclear(res);
                    return false;
                }
                PQclear(res);
            }
        }
    }

    return true;
}
