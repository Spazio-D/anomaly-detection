#include "main.h"

bool saveAnomalySQL(std::map<std::string, std::vector<Average>> &averages, std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn){
    
    PGresult *res;
    std::string query;
    std::string isAnomaly;

    for(auto element : averages){
        for(Average average : averages[element.first]){

            isAnomaly = dataVector[element.first][average.lastSampleTime].isAverageAnomaly ? "TRUE" : "FALSE";
            query = "INSERT INTO anomalyAverageTable (sensorID, firstSampleTime, isAnomaly) VALUES ('"+ average.sensorID 
                    + "', " + std::to_string(average.firstSampleTime) + ", " + isAnomaly + ")";
        
            res = PQexec(conn, query.c_str());
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
