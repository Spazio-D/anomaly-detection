#include "main.h"

bool saveAverageInPostgreSQL(std::map<std::string, double> &averages, size_t firstSampleTime, PGconn *conn){

    PGresult *res = PQexec(conn, "BEGIN");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Errore durante l'inizio della transazione: " << PQresultErrorMessage(res) << std::endl;
        PQclear(res);
        return false;
    }

    PQclear(res);       

    for (auto average : averages) {
    
        std::string query = "INSERT INTO averageTable (sensorID, value, firstSampleTime, lastSampleTime) VALUES ('"
                            + average.first + "', " + std::to_string(average.second) + ", " + std::to_string(firstSampleTime) 
                            + ", " + std::to_string(firstSampleTime + W) + ")";
    
        res = PQexec(conn, query.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
            PQclear(res);
            return false;
        }
        PQclear(res);
    
    }

    //Concludi la transazione
    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Errore durante la conclusione della transazione: " << PQresultErrorMessage(res) << std::endl;
        return false;
    }
    PQclear(res);
    return true;
}
