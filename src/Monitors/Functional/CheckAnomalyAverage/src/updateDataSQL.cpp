#include "main.h"

bool updateDataSQL(PGconn *conn){

    // Query per la selezione dei sampleTime dalla tabella averageTable
    std::string query = "SELECT firstSampleTime, lastSampleTime FROM averageTable;";
    PGresult *resWindow = PQexec(conn, query.c_str());
    if (PQresultStatus(resWindow) != PGRES_TUPLES_OK || PQntuples(resWindow) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione dei sampleTime per la finestra: " << PQresultErrorMessage(resWindow) << std::endl;
        PQclear(resWindow);
        return false;
    }   

    // Calcolo della dimensione della finestra temporale
    std::string firstSampleTime1 = PQgetvalue(resWindow, 0, 0);
    std::string lastSampleTime = PQgetvalue(resWindow, 0, 1);
    int windowSize = std::stoi(lastSampleTime) - std::stoi(firstSampleTime1) + 1;
    PQclear(resWindow);

    // Query per la selezione dei dati dei sensori
    query = "SELECT sensorID, sampleTime, value FROM dataTable;";
    PGresult *resSelect = PQexec(conn, query.c_str());
    if (PQresultStatus(resSelect) != PGRES_TUPLES_OK) {
        std::cerr << "Errore nella query di selezione dei dati: " << PQerrorMessage(conn) << std::endl;
        PQclear(resSelect);
        return false;
    }

    // Scorrimento dei dati dei sensori
    int rows = PQntuples(resSelect);
    for (int i = 0; i < rows; i++) {

        std::string sensorID = PQgetvalue(resSelect, i, 0);
        std::string firstsampletime = std::to_string(std::stoi(PQgetvalue(resSelect, i, 1)) - windowSize + 1);
        double value = PQgetisnull(resSelect, i, 2) ? std::nan("") : std::stod(PQgetvalue(resSelect, i, 2));

        query = "SELECT isAnomaly, upperThreshold, lowerThreshold FROM anomalyAverageTable WHERE sensorID = '" 
                + sensorID + "' AND firstSampleTime = " + firstsampletime + ";";
        PGresult *resSelectAnomaly = PQexec(conn, query.c_str());
        if (PQresultStatus(resSelectAnomaly) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nella query di selezione dei dati: " << PQerrorMessage(conn) << std::endl;
            PQclear(resSelectAnomaly);
            return false;
        }
        if(PQntuples(resSelectAnomaly) == 0){
            continue;
        }

        std::string x = PQgetvalue(resSelectAnomaly, 0, 0);
        bool isAnomaly = x == "t" ? true : false;
        double upperThreshold = PQgetisnull(resSelectAnomaly, 0, 1) ? std::nan("") : std::stod(PQgetvalue(resSelectAnomaly, 0, 1));
        double lowerThreshold = PQgetisnull(resSelectAnomaly, 0, 2) ? std::nan("") : std::stod(PQgetvalue(resSelectAnomaly, 0, 2));
        std::string check;
        
        if(std::isnan(value) && !isAnomaly){
           check = "TRUE";
        } else if(std::isnan(value) && isAnomaly){
            check = "FALSE";
        } else if(((lowerThreshold <= value && value <= upperThreshold)  && !isAnomaly) || (!(lowerThreshold <= value && value <= upperThreshold)  && isAnomaly)){
            check = "TRUE";
        } else {
            check = "FALSE";
        }
        std::cout << "check: " << check << std::endl;
        std::cout << std::endl;

        query = "INSERT INTO MonitorAnomalyAverageTable (sensorID, firstSampleTime, isRight) VALUES ('" 
                + sensorID + "', " + firstsampletime + ", " + check + ");"; 
        PGresult *resInsert = PQexec(conn, query.c_str());
        if (PQresultStatus(resInsert) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'inserimento dei dati: " << PQerrorMessage(conn) << std::endl;
            PQclear(resInsert);
            return false;
        }
    }

    PQclear(resSelect);
    return true;
}