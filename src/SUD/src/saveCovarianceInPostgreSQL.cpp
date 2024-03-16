#include "main.h"

bool saveCovarianceInPostgreSQL(std::vector<std::vector<double>> &covariances, size_t firstSampleTime, PGconn *conn){
      
    // Scorrimento della matrice delle covarianze
    for (size_t i=0; i<covariances.size(); i++) {
        for (size_t j=0; j<covariances[i].size(); j++) {

            std::string value;
            if(std::isnan(covariances[i][j])){
                value = "NULL";
            }else{
                value = std::to_string(covariances[i][j]);
            }

            // Creazione query e inserimento covarianza nel Database
            std::string query = "INSERT INTO covarianceTable (sensorID1, sensorID2, value, firstSampleTime, lastSampleTime) VALUES ('SAC"
                                + std::to_string(i) + "', 'SAC" + std::to_string(j) + "', " + value + ", " 
                                + std::to_string(firstSampleTime) + ", " + std::to_string(firstSampleTime + WINDOW_SIZE-1) + ")";
            PGresult *res = PQexec(conn, query.c_str());
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
