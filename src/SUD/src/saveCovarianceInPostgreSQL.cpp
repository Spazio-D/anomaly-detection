#include "main.h"

bool saveCovarianceInPostgreSQL(std::vector<std::vector<double>> &covariances, size_t firstSampleTime, PGconn *conn){

    PGresult *res;      
    std::string value;
    
    for (size_t i=0; i<covariances.size(); i++) {
        for (size_t j=0; j<covariances[i].size(); j++) {

            if(std::isnan(covariances[i][j])){
                value = "NULL";
            }else{
                value = std::to_string(covariances[i][j]);
            }

            std::string query = "INSERT INTO covarianceTable (sensorID1, sensorID2, value, firstSampleTime, lastSampleTime) VALUES ('SAC"
                                + std::to_string(i) + "', 'SAC" + std::to_string(j) + "', " + value + ", " 
                                + std::to_string(firstSampleTime) + ", " + std::to_string(firstSampleTime + W-1) + ")";
        
            res = PQexec(conn, query.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
                PQclear(res);
                return false;
            }

            PQclear(res);
        }
    }

    return true;
}
