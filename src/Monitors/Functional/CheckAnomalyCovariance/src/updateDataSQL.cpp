#include "main.h"

bool updateDataSQL(PGconn *conn){
    
    // Query per la selezione dei dati dalla tabella delle anomalie delle covarianze
    std::string query = "SELECT sensorID1, sensorID2, value, threshold, isAnomaly, firstSampleTime FROM anomalyCovarianceTable;";
    PGresult *resSelect = PQexec(conn, query.c_str());
    if (PQresultStatus(resSelect) != PGRES_TUPLES_OK || PQntuples(resSelect) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione anomalie di covarianza: " << PQresultErrorMessage(resSelect) << std::endl;
        PQclear(resSelect);
        return false;
    }

    int rows = PQntuples(resSelect);
    for (int i = 0; i < rows; i++) {
        std::string sensorID1 = PQgetvalue(resSelect, i, 0);
        std::string sensorID2 = PQgetvalue(resSelect, i, 1);
        double value = PQgetisnull(resSelect, i, 2) ? std::nan("") : std::stod(PQgetvalue(resSelect, i, 2));
        double threshold = std::stod(PQgetvalue(resSelect, i, 3));
        std::string x = PQgetvalue(resSelect, i, 4);
        bool isAnomaly = x == "t" ? true : false;
        std::string firstSampleTime = PQgetvalue(resSelect, i, 5);
        bool check = value < threshold && value > -threshold ? true : false; 
        
        std::string isRight;
        if(std::isnan(value) && !isAnomaly){
            isRight = "TRUE";
        }else if(std::isnan(value) && isAnomaly){
            isRight = "FALSE";
        }else if(check && !isAnomaly){
            isRight = "TRUE";
        }else if(check && isAnomaly){
            isRight = "FALSE";
        }else if (!check && !isAnomaly){
            isRight = "FALSE";
        }else if (!check && isAnomaly){
            isRight = "TRUE";
        }
        
        query = "INSERT INTO MonitorAnomalyCovarianceTable (sensorID1, sensorID2, firstSampleTime, isRight) VALUES ('" + sensorID1 + "', '" + sensorID2 + "', " + firstSampleTime + ", " + isRight + ");";
        PGresult *resInsert = PQexec(conn, query.c_str());
        if (PQresultStatus(resInsert) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'esecuzione della query di inserimento dei dati: " << PQresultErrorMessage(resInsert) << std::endl;
            PQclear(resInsert);
            PQclear(resSelect);
            return false;
        }

        PQclear(resInsert); 
    }

    PQclear(resSelect);
    return true;
}