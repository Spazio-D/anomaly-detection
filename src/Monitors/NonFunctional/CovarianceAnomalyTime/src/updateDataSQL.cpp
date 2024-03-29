#include "main.h"

bool updateDataSQL(PGconn *conn){
    
    // Query per la selezione dei deltaTime delle anomalie delle covarianze
    std::string query = "SELECT sensorID1, sensorID2, firstSampleTime, deltaTime FROM anomalyCovarianceTable";
    PGresult *resSelect = PQexec(conn, query.c_str());
    if (PQresultStatus(resSelect) != PGRES_TUPLES_OK || PQntuples(resSelect) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione dei sensorID, firstSampleTime e deltaTime per la tabella anomalyCovarianceTable: " << PQresultErrorMessage(resSelect) << std::endl;
        PQclear(resSelect);
        return false;
    }

    // Scorrimento delle tuple restituite dalla query dei deltaTime
    for(int i = 0; i < PQntuples(resSelect); i++){

        std::string sensorID1 = PQgetvalue(resSelect, i, 0);
        std::string sensorID2 = PQgetvalue(resSelect, i, 1);
        std::string firstSampleTime = PQgetvalue(resSelect, i, 2);
        std::string deltaTime = PQgetvalue(resSelect, i, 3);

        // Calcolo del deltaTime in millisecondi
        int hours, minutes, seconds, milliseconds;
        sscanf(deltaTime.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
        milliseconds = std::stoi(deltaTime.substr(9,3));
        milliseconds += (hours * 3600 + minutes * 60 + seconds) * 1000;

        // Rivelamento dell'anomalia di tempo e aggiornamento del campo exceededTime
        std::string exceededTime = milliseconds > MAXTIME ? "TRUE" : "FALSE";
        query = "UPDATE anomalyCovarianceTable SET exceededTime = " + exceededTime + " WHERE sensorID1 = '" + sensorID1 + "' AND sensorID2 = '" + sensorID2 + "' AND firstSampleTime = " + firstSampleTime + ";";
        PGresult *resUpdate = PQexec(conn, query.c_str());
        if( PQresultStatus(resUpdate) != PGRES_COMMAND_OK){
            std::cerr << "Errore durante l'esecuzione della query di update " << PQresultErrorMessage(resUpdate) << std::endl;
            PQclear(resUpdate);
            return false;
        }

        PQclear(resUpdate);
    }

    PQclear(resSelect);
    return true;
}