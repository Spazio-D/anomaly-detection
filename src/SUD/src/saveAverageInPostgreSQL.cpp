#include "main.h"

bool saveAverageInPostgreSQL(std::map<std::string, double> &averages, size_t firstSampleTime, PGconn *conn){

    PGresult *res;
    std::string value;
    
    for (auto average : averages) {
        
        std::cout << average.second << std::endl;
        if(std::isnan(average.second)){
            value = "NULL";
        }else{
            value = std::to_string(average.second);
        }
        
        std::string query = "INSERT INTO averageTable (sensorID, value, firstSampleTime, lastSampleTime) VALUES ('"
                            + average.first + "', " + value + ", " + std::to_string(firstSampleTime) 
                            + ", " + std::to_string(firstSampleTime + W-1) + ")";
    
        res = PQexec(conn, query.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
            PQclear(res);
            return false;
        }
        
        PQclear(res);
    
    }

    return true;
}
