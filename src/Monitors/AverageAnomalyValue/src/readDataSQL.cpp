#include "main.h"

bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, double> averages, PGconn *conn){

    PGresult *resSensorsID = PQexec(conn, "SELECT DISTINCT sensorID FROM datatable ORDER BY sensorID;");

    if (PQresultStatus(resSensorsID) != PGRES_TUPLES_OK) {
        std::cerr << "Errore nell'esecuzione della select su dataTable dei sensorID: " << PQresultErrorMessage(resSensorsID) << std::endl;
        PQclear(resSensorsID);
        PQfinish(conn);
        return false;
    }

    std::string sensorID;
    std::string query;
    PGresult *resSensorData;
    Data data;
    std::string average;
    for (int i = 0; i <PQntuples(resSensorsID); i++) {

        sensorID = PQgetvalue(resSensorsID, i, 0);
        query = "SELECT sampleTime, value FROM dataTable WHERE sensorID = '" + sensorID + "'";
        resSensorData = PQexec(conn, query.c_str());

        if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su dataTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resSensorData) << std::endl;
            PQclear(resSensorData);
            PQclear(resSensorsID);
            PQfinish(conn);
            return false;
        }

        for (int j = 0; j <PQntuples(resSensorData); j++) {

            data.sensorID = sensorID;
            data.sampleTime = PQgetvalue(resSensorData, j, 0);
            data.value = PQgetvalue(resSensorData, j, 1);
            dataVector[sensorID].push_back(data);

        }

        PQclear(resSensorData);
        query = "SELECT value FROM averageTable WHERE sensorID = '" + sensorID + "'";
        resSensorData = PQexec(conn, query.c_str());

        if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su averageTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resSensorData) << std::endl;
            PQclear(resSensorData);
            PQclear(resSensorsID);
            PQfinish(conn);
            return false;
        }

        average = PQgetvalue(resSensorData, 0, 0);
        if(average == ""){
            averages[sensorID] = std::nan("");
        }else{
            averages[sensorID] = std::stod(average);
        }

        PQclear(resSensorData);
        
    }

    PQclear(resSensorsID);
    PQfinish(conn);
    return true;

}