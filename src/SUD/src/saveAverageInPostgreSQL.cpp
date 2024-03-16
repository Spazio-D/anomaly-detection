#include "main.h"

bool saveAverageInPostgreSQL(std::map<std::string, double> &averages, size_t firstSampleTime, PGconn *conn){
    
    // Scorrimento delle medie
    for (auto average : averages) {
        
        std::string value;
        if(std::isnan(average.second)){
            value = "NULL";
        }else{
            value = std::to_string(average.second);
        }
        
        // Creazione query e inserimento media nel Database
        std::string query = "INSERT INTO averageTable (sensorID, value, firstSampleTime, lastSampleTime) VALUES ('"
                + average.first + "', " + value + ", " + std::to_string(firstSampleTime) 
                + ", " + std::to_string(firstSampleTime + WINDOW_SIZE-1) + ")";
        PGresult *res = PQexec(conn, query.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
            PQclear(res);
            return false;
        }
        
        PQclear(res);
    }

    return true;
}
