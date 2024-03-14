#include "main.h"

bool updateDataSQL(PGconn *conn){

    std::string query = "SELECT firstSampleTime, lastSampleTime FROM covarianceTable;";
    PGresult *resWindow = PQexec(conn, query.c_str());
    if (PQresultStatus(resWindow) != PGRES_TUPLES_OK || PQntuples(resWindow) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione dei sampleTime per la finestra: " << PQresultErrorMessage(resWindow) << std::endl;
        PQclear(resWindow);
        return false;
    }   

    std::string firstSampleTime = PQgetvalue(resWindow, 0, 0);
    std::string lastSampleTime = PQgetvalue(resWindow, 0, 1);
    int windowSize = std::stoi(lastSampleTime) - std::stoi(firstSampleTime) + 1;
    PQclear(resWindow);

    query = "SELECT sensorID1, sensorID2, firstSampleTime FROM anomalyCovarianceTable;";
    PGresult *resCovariance = PQexec(conn, query.c_str());
    if( PQresultStatus(resCovariance) != PGRES_TUPLES_OK || PQntuples(resCovariance) == 0){
        std::cerr << "Errore durante l'esecuzione della query di selezione dei sensorID e firstSampleTime per la tabella anomalyCovarianceTable: " << PQresultErrorMessage(resCovariance) << std::endl;
        PQclear(resCovariance);
        return false;
    }

    for(int i = 0; i < PQntuples(resCovariance); i++){

        std::string sensorID1 = PQgetvalue(resCovariance, i, 0);
        std::string sensorID2 = PQgetvalue(resCovariance, i, 1);
        std::string firstSampleTime = PQgetvalue(resCovariance, i, 2);
        std::string sampleTime = std::to_string(std::stoi(firstSampleTime) + windowSize -1);

        //query = "WITH readTimes AS ( SELECT (SELECT readTime FROM dataTable WHERE sensorID = '" + sensorID1 + "' AND sampleTime = " + sampleTime + ") AS readTime1, (SELECT readTime FROM dataTable WHERE sensorID = '" + sensorID2 + "' AND sampleTime = " + sampleTime + ") AS readTime2 FROM anomalyCovarianceTable) UPDATE anomalyCovarianceTable SET deltaTime = (SELECT INTERVAL '1 second' * ( EXTRACT( HOUR FROM LEAST(rt.readTime1, rt.readTime2)) * 3600 + EXTRACT(MINUTE FROM LEAST(rt.readTime1, rt.readTime2)) * 60 + EXTRACT(SECOND FROM LEAST(rt.readTime1, rt.readTime2)) - EXTRACT(HOUR FROM GREATEST(rt.readTime1, rt.readTime2)) * 3600 - EXTRACT(MINUTE FROM GREATEST(rt.readTime1, rt.readTime2)) * 60 - EXTRACT(SECOND FROM GREATEST(rt.readTime1, rt.readTime2))) FROM readTimes rt ) WHERE sensorID1 = '" + sensorID1 + "' AND sensorID2 = '" + sensorID2 + "' AND firstSampleTime = " + sampleTime + ";";  
        //query = "UPDATE anomalyCovarianceTable SET deltaTime = (SELECT INTERVAL '1 second' * ( EXTRACT( HOUR FROM LEAST(rt.readTime1, rt.readTime2)) * 3600 + EXTRACT(MINUTE FROM LEAST(rt.readTime1, rt.readTime2)) * 60 + EXTRACT(SECOND FROM LEAST(rt.readTime1, rt.readTime2)) - EXTRACT(HOUR FROM GREATEST(rt.readTime1, rt.readTime2)) * 3600 - EXTRACT(MINUTE FROM GREATEST(rt.readTime1, rt.readTime2)) * 60 - EXTRACT(SECOND FROM GREATEST(rt.readTime1, rt.readTime2))) FROM (SELECT readTime FROM dataTable WHERE sensorID = '" + sensorID1 + "' AND sampleTime = " + sampleTime + ") AS readTime1)";";  
        
        query = "UPDATE anomalyCovarianceTable SET deltaTime = (SELECT INTERVAL '1 second' * (EXTRACT(HOUR FROM GREATEST(rt1.readTime, rt2.readTime)) * 3600 + EXTRACT(MINUTE FROM GREATEST(rt1.readTime, rt2.readTime)) * 60 + EXTRACT(SECOND FROM GREATEST(rt1.readTime, rt2.readTime)) - EXTRACT(HOUR FROM LEAST(rt1.readTime, rt2.readTime)) * 3600 - EXTRACT(MINUTE FROM LEAST(rt1.readTime, rt2.readTime)) * 60 - EXTRACT(SECOND FROM LEAST(rt1.readTime, rt2.readTime))) FROM ((SELECT readTime FROM dataTable WHERE sensorID = '" + sensorID1 + "' AND sampleTime = " + sampleTime + ") AS rt1, (SELECT readTime FROM dataTable WHERE sensorID = '" + sensorID2 + "' AND sampleTime = " + sampleTime + ") AS rt2) WHERE sensorID1 = '" + sensorID1 + "' AND sensorID2 = '" + sensorID2 + "' AND firstSampleTime = " + sampleTime + ";";
        PGresult *resTime = PQexec(conn, query.c_str());
        if (PQresultStatus(resTime) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'esecuzione della query di aggiornamento del deltaTime: " << PQresultErrorMessage(resTime) << std::endl;
            PQclear(resTime);
            return false;
        }


        // query = "SELECT sensorID, readTime FROM dataTable WHERE sensorID = '" + sensorID1 + "' AND sampleTime = " + sampleTime + ";";
        // PGresult *resTime1 = PQexec(conn, query.c_str());
        // if( PQresultStatus(resTime1) != PGRES_TUPLES_OK || PQntuples(resTime1) == 0){
        //     std::cerr << "Errore durante l'esecuzione della query di selezione del readTime per il sensore " << sensorID1 << " e il sampleTime " << sampleTime << ": " << PQresultErrorMessage(resTime1) << std::endl;
        //     PQclear(resTime1);
        //     return false;
        // }

        // query = "SELECT sensorID, readTime FROM dataTable WHERE sensorID = '" + sensorID2 + "' AND sampleTime = " + sampleTime +";";
        // PGresult *resTime2 = PQexec(conn, query.c_str());
        // if( PQresultStatus(resTime2) != PGRES_TUPLES_OK || PQntuples(resTime2) == 0){
        //     std::cerr << "Errore durante l'esecuzione della query di selezione del readTime per il sensore " << sensorID2 << " e il sampleTime " << sampleTime << ": " << PQresultErrorMessage(resTime2) << std::endl;
        //     PQclear(resTime2);
        //     return false;
        // }

    }





    return true;

}