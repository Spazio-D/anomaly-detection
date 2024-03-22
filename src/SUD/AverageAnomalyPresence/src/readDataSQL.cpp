#include "main.h"

bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, PGconn *conn){

    // Selezione dei sensorID
    std::string query = "SELECT DISTINCT sensorID FROM datatable;";
    PGresult *resSensorsID = PQexec(conn, query.c_str());
    if (PQresultStatus(resSensorsID) != PGRES_TUPLES_OK) {
        std::cerr << "Errore nell'esecuzione della select su dataTable dei sensorID: " << PQresultErrorMessage(resSensorsID) << std::endl;
        PQclear(resSensorsID);
        return false;
    }

    // Scorrimento dei sensorID
    for (int i = 0; i <PQntuples(resSensorsID); i++) {

        std::string sensorID = PQgetvalue(resSensorsID, i, 0);

        // Selezione dei dati dello specifico sensorID
        query = "SELECT sampleTime, value FROM dataTable WHERE sensorID = '" + sensorID + "'";
        PGresult *resSensorData = PQexec(conn, query.c_str());
        if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su dataTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resSensorData) << std::endl;
            PQclear(resSensorData);
            PQclear(resSensorsID);
            return false;
        }

        // Scorrimento dei valori del sensorID e salvataggio nella struttura dati
        for (int j = 0; j <PQntuples(resSensorData); j++) {
            
            Data data;
            data.sensorID = sensorID;
            data.sampleTime = PQgetvalue(resSensorData, j, 0);
            data.value = PQgetvalue(resSensorData, j, 1);
            dataVector[sensorID].push_back(data);

        }

        PQclear(resSensorData);

        // Selezione dei valori dalla tabella delle medie
        query = "SELECT value, firstSampleTime, lastSampleTime FROM averageTable WHERE sensorID = '" + sensorID + "'";
        resSensorData = PQexec(conn, query.c_str());
        if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su averageTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resSensorData) << std::endl;
            PQclear(resSensorData);
            PQclear(resSensorsID);
            return false;
        }

        // Scorrimento dei valori delle medie e creazione e salvataggio nella struttura dati
        std::vector<Average> averageVector;
        for(int j = 0; j < PQntuples(resSensorData); j++){
            
            Average average;
            average.sensorID = sensorID;
            std::string check = PQgetvalue(resSensorData, j, 0);
            if(check == ""){
                average.value = std::nan("");
            }else{
                average.value = std::stod(check);
            }
            average.firstSampleTime = std::stoi(PQgetvalue(resSensorData, j, 1));
            average.lastSampleTime = std::stoi(PQgetvalue(resSensorData, j, 2));
            averageVector.push_back(average);

        }

        averages[sensorID] = averageVector;
        PQclear(resSensorData);
    }

    PQclear(resSensorsID);
    return true;
}