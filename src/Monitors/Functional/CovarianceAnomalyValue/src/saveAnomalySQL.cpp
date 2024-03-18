#include "main.h"

bool saveAnomalySQL(std::vector<std::vector<std::vector<AnomalyCovariance>>> &covarianceAnomalyVector, PGconn *conn){
    
    // Scorrimento della matrice dei valori delle anomalie delle covarianze
    for(size_t i = 0; i < covarianceAnomalyVector.size(); i++){
        for(size_t j = 0; j < covarianceAnomalyVector[i].size(); j++){
            for(size_t k = 0; k < covarianceAnomalyVector[i][j].size(); k++){

                AnomalyCovariance anomaly = covarianceAnomalyVector[i][j][k];
                std::string value = std::isnan(anomaly.value) ? "NULL" : std::to_string(anomaly.value);

                // Query per inserimento del valore anomalo nel database
                std::string query = "INSERT INTO anomalyCovarianceTable (sensorID1, sensorID2, firstSampleTime, value) VALUES ('" + anomaly.sensorID1 + "', '" + anomaly.sensorID2 + "', " + std::to_string(k) + ", " + value + ")";
                PGresult *res = PQexec(conn, query.c_str());
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
