#include "main.h"

bool saveAnomalySQL(std::map<std::string, std::vector<Average>> &averages, std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn){

    // Scorrimento dei valori delle medie
    for(auto element : averages){
        for(Average average : element.second){

            // Query per il salvataggio sul database del controllo anomalie e delle threshold
            std::string isAnomaly = dataVector[element.first][average.lastSampleTime].isAverageAnomaly ? "TRUE" : "FALSE";
            std::string upperThreshold = std::isnan(dataVector[element.first][average.lastSampleTime].upperThreshold) ? "NULL" : std::to_string(dataVector[element.first][average.lastSampleTime].upperThreshold);
            std::string lowerThreshold = std::isnan(dataVector[element.first][average.lastSampleTime].lowerThreshold) ? "NULL" : std::to_string(dataVector[element.first][average.lastSampleTime].lowerThreshold);
            std::string query = "INSERT INTO anomalyAverageTable (sensorID, firstSampleTime, isAnomaly, upperThreshold, lowerThreshold) VALUES ('"+ average.sensorID 
                    + "', " + std::to_string(average.firstSampleTime) + ", " + isAnomaly + ", " + upperThreshold + ", " + lowerThreshold + ")";
        
            PGresult *res = PQexec(conn, query.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::cerr << "Errore durante l'esecuzione della query di inserimento anomalie di media: " << PQresultErrorMessage(res) << std::endl;
                PQclear(res);
                return false;
            }
            
            PQclear(res);
        }
    }

    return true;
}
