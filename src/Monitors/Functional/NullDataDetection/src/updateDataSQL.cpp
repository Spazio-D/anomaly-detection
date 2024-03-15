#include "main.h"

bool updateDataSQL(PGconn *conn){

    std::string query = "SELECT DISTINCT sensorID FROM dataTable;";
    PGresult *res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Errore nella query di selezione dei sensorID: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return false;
    }

    size_t rows = PQntuples(res);
    for(size_t i = 0; i < rows; i++){
        std::string sensorID = PQgetvalue(res, i, 0);

        query = "SELECT sampleTime, value FROM dataTable WHERE sensorID = '" + sensorID + "' ORDER BY sampleTime;";
        PGresult *res2 = PQexec(conn, query.c_str());
        if (PQresultStatus(res2) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nella query di selezione dei sampleTime e value: " << PQerrorMessage(conn) << std::endl;
            PQclear(res2);
            PQclear(res);
            return false;
        }

        size_t rows2 = PQntuples(res2);
        int nullStreak = 0;
        for(size_t j = 0; j < rows2; j++){
            std::string sampleTime = PQgetvalue(res2, j, 0);
            std::string value = PQgetvalue(res2, j, 1);
            
            //std::cout << "SensorID: " << sensorID << " SampleTime: " << sampleTime << " Value: " << value << std::endl;

            if(value == "" && j != rows2 - 1){
                nullStreak++;
                continue;
            }else if(value == "" && j == rows2 - 1){
                nullStreak++;
                sampleTime = std::to_string(std::stoi(sampleTime) + 1);
            }
            
            if(nullStreak == 0){
                continue;
            }

            std::string firstSampleTime = std::to_string(std::stoi(sampleTime) - nullStreak);
            query = "INSERT INTO missingDataTable (sensorID, firstSampleTime, lastSampleTime, nullStreak) VALUES ('" + sensorID + "', " + firstSampleTime + ", " + sampleTime + ", " + std::to_string(nullStreak) + ");";
            PGresult *res3 = PQexec(conn, query.c_str());
            if (PQresultStatus(res3) != PGRES_COMMAND_OK) {
                std::cerr << "Errore nell'inserimento dei dati mancanti: " << PQerrorMessage(conn) << std::endl;
                PQclear(res3);
                PQclear(res2);
                PQclear(res);
                return false;
            }

            nullStreak = 0;
            PQclear(res3);

        }
        
        PQclear(res2);
        
    }

    PQclear(res);
    return true;
}