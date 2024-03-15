#include "main.h"

bool updateDataSQL(PGconn *conn){

    std::string query = "SELECT firstSampleTime, lastSampleTime FROM averageTable;";
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

    query = "UPDATE anomalyAverageTable SET deltaTime = (anomalyAverageTable.detectionTime - dataTable.readTime) FROM dataTable WHERE anomalyAverageTable.sensorID = dataTable.sensorID AND dataTable.sampleTime = anomalyAverageTable.firstSampleTime + " + std::to_string(windowSize-1) + ";";
    PGresult *resTime = PQexec(conn, query.c_str());

    if (PQresultStatus(resTime) != PGRES_COMMAND_OK) {
        std::cerr << "Errore durante l'esecuzione della query di aggiornamento del deltaTime: " << PQresultErrorMessage(resTime) << std::endl;
        PQclear(resTime);
        return false;
    }

    PQclear(resTime);
    return true;
}