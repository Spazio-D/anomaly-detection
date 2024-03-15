#include "main.h"

bool updateDataSQL(PGconn *conn){
    
    std::string query = "SELECT sensorID, firstSampleTime, deltaTime FROM anomalyAverageTable";
    PGresult *resSelect = PQexec(conn, query.c_str());
    if (PQresultStatus(resSelect) != PGRES_TUPLES_OK || PQntuples(resSelect) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione dei sensorID, firstSampleTime e deltaTime per la tabella anomalyAverageTable: " << PQresultErrorMessage(resSelect) << std::endl;
        PQclear(resSelect);
        return false;
    }

    for(int i = 0; i < PQntuples(resSelect); i++){

        std::string sensorID = PQgetvalue(resSelect, i, 0);
        std::string firstSampleTime = PQgetvalue(resSelect, i, 1);

        std::string deltaTime = PQgetvalue(resSelect, i, 2);
        //std::cout << "deltaTime: " << deltaTime << std::endl;
        int hours, minutes, seconds, milliseconds;
        sscanf(deltaTime.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
        milliseconds = std::stoi(deltaTime.substr(9,3));
        //std::cout << "hours: " << hours << " minutes: " << minutes << " seconds: " << seconds << " milliseconds: " << milliseconds << std::endl;
        milliseconds += (hours * 3600 + minutes * 60 + seconds) * 1000;
        //std::cout << "milliseconds: " << milliseconds << std::endl;

        std::string exceededTime = milliseconds > MAXTIME ? "TRUE" : "FALSE";
        //std::cout << "exceededTime: " << exceededTime << std::endl;

        query = "UPDATE anomalyAverageTable SET exceededTime = " + exceededTime + " WHERE sensorID = '" + sensorID + "' AND firstSampleTime = " + firstSampleTime + ";";
        PGresult *resUpdate = PQexec(conn, query.c_str());
        if( PQresultStatus(resUpdate) != PGRES_COMMAND_OK){
            std::cerr << "Errore durante l'esecuzione della query di update" << PQresultErrorMessage(resUpdate) << std::endl;
            PQclear(resUpdate);
            return false;
        }

        PQclear(resUpdate);
        
    }

    PQclear(resSelect);
    return true;
}