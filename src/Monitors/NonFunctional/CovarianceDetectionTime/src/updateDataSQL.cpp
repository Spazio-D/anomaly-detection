#include "main.h"

bool updateDataSQL(PGconn *conn){

    // Query per la selezione dei sampleTime
    std::string query = "SELECT firstSampleTime, lastSampleTime FROM covarianceTable;";
    PGresult *resWindow = PQexec(conn, query.c_str());
    if (PQresultStatus(resWindow) != PGRES_TUPLES_OK || PQntuples(resWindow) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione dei sampleTime per la finestra: " << PQresultErrorMessage(resWindow) << std::endl;
        PQclear(resWindow);
        return false;
    }   

    // Calcolo della dimensione della finestra temporale
    std::string firstSampleTime = PQgetvalue(resWindow, 0, 0);
    std::string lastSampleTime = PQgetvalue(resWindow, 0, 1);
    int windowSize = std::stoi(lastSampleTime) - std::stoi(firstSampleTime) + 1;
    PQclear(resWindow);

    // Query per la selezione dei detectionTime delle covarianze
    query = "SELECT sensorID1, sensorID2, firstSampleTime, detectionTime FROM anomalyCovarianceTable;";
    PGresult *resCovariance = PQexec(conn, query.c_str());
    if( PQresultStatus(resCovariance) != PGRES_TUPLES_OK || PQntuples(resCovariance) == 0){
        std::cerr << "Errore durante l'esecuzione della query di selezione dei sensorID e firstSampleTime per la tabella anomalyCovarianceTable: " << PQresultErrorMessage(resCovariance) << std::endl;
        PQclear(resCovariance);
        return false;
    }

    // Scorrimento delle tuple restituite dalla query dei detectionTime
    for(int i = 0; i < PQntuples(resCovariance); i++){

        std::string sensorID1 = PQgetvalue(resCovariance, i, 0);
        std::string sensorID2 = PQgetvalue(resCovariance, i, 1);
        std::string firstSampleTime = PQgetvalue(resCovariance, i, 2);
        std::string detectionTime = PQgetvalue(resCovariance, i, 3);
        std::string sampleTime = std::to_string(std::stoi(firstSampleTime) + windowSize -1);

        // Query per la selezione dei readTime del primo sensore
        query = "SELECT readTime FROM dataTable WHERE sensorID = '" + sensorID1 + "' AND sampleTime = " + sampleTime + ";";
        PGresult *resTime1 = PQexec(conn, query.c_str());
        if( PQresultStatus(resTime1) != PGRES_TUPLES_OK || PQntuples(resTime1) == 0){
            std::cerr << "Errore durante l'esecuzione della query di selezione del readTime per il sensore " << sensorID1 << " e il sampleTime " << sampleTime << ": " << PQresultErrorMessage(resTime1) << std::endl;
            PQclear(resTime1);
            return false;
        }

        std::string readTime1 = PQgetvalue(resTime1, 0, 0);

        // Query per la selezione dei readTime del secondo sensore
        query = "SELECT readTime FROM dataTable WHERE sensorID = '" + sensorID2 + "' AND sampleTime = " + sampleTime +";";
        PGresult *resTime2 = PQexec(conn, query.c_str());
        if( PQresultStatus(resTime2) != PGRES_TUPLES_OK || PQntuples(resTime2) == 0){
            std::cerr << "Errore durante l'esecuzione della query di selezione del readTime per il sensore " << sensorID2 << " e il sampleTime " << sampleTime << ": " << PQresultErrorMessage(resTime2) << std::endl;
            PQclear(resTime2);
            return false;
        }

        std::string readTime2 = PQgetvalue(resTime2, 0, 0);

        // Query per l'aggiornamento dei dati nella tabella delle anomalie delle covarianze
        query = "UPDATE anomalyCovarianceTable SET deltaTime = '" + detectionTime + "' - (SELECT LEAST(TIME '" + readTime1 + "', TIME '" + readTime2 + "')) WHERE sensorID1 = '" + sensorID1 + "' AND sensorID2 = '" + sensorID2 + "' AND firstSampleTime = " + firstSampleTime + ";";
        PGresult *resUpdate = PQexec(conn, query.c_str());
        if( PQresultStatus(resUpdate) != PGRES_COMMAND_OK){
            std::cerr << "Errore durante l'esecuzione della query di update" << PQresultErrorMessage(resUpdate) << std::endl;
            PQclear(resUpdate);
            return false;
        }

        PQclear(resTime1);
        PQclear(resTime2);
        PQclear(resUpdate);
    }
        
    return true;
}