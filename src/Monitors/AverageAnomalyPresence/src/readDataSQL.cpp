#include "main.h"

bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, PGconn *conn){

    PGresult *resSensorsID = PQexec(conn, "SELECT DISTINCT sensorID FROM datatable;");

    if (PQresultStatus(resSensorsID) != PGRES_TUPLES_OK) {
        std::cerr << "Errore nell'esecuzione della select su dataTable dei sensorID: " << PQresultErrorMessage(resSensorsID) << std::endl;
        PQclear(resSensorsID);
        return false;
    }

    std::string sensorID;
    std::string query;
    PGresult *resSensorData;
    Data data;
    Average average;
    std::string check;
    for (int i = 0; i <PQntuples(resSensorsID); i++) {

        sensorID = PQgetvalue(resSensorsID, i, 0);
        query = "SELECT sampleTime, value FROM dataTable WHERE sensorID = '" + sensorID + "'";
        resSensorData = PQexec(conn, query.c_str());

        if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su dataTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resSensorData) << std::endl;
            PQclear(resSensorData);
            PQclear(resSensorsID);
            return false;
        }

        for (int j = 0; j <PQntuples(resSensorData); j++) {

            data.sensorID = sensorID;
            data.sampleTime = PQgetvalue(resSensorData, j, 0);
            data.value = PQgetvalue(resSensorData, j, 1);
            dataVector[sensorID].push_back(data);

        }

        PQclear(resSensorData);
        query = "SELECT value, firstSampleTime, lastSampleTime FROM averageTable WHERE sensorID = '" + sensorID + "'";
        resSensorData = PQexec(conn, query.c_str());

        if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su averageTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resSensorData) << std::endl;
            PQclear(resSensorData);
            PQclear(resSensorsID);
            return false;
        }

        std::vector<Average> averageVector;

        for(int j = 0; j < PQntuples(resSensorData); j++){
            
            average.sensorID = sensorID;
            check = PQgetvalue(resSensorData, j, 0);
            if(check == ""){
                average.value = std::nan("");
            }else{
                average.value = std::stod(check);
            }
            average.firstSampleTime = std::stoi(PQgetvalue(resSensorData, j, 1));
            average.lastSampleTime = std::stoi(PQgetvalue(resSensorData, j, 2));
            averageVector.push_back(average);
            //std::cout << "sensorID:" << average.sensorID << " value:" << average.value << " firstSampleTime:" << average.firstSampleTime << " lastSampleTime:" << average.lastSampleTime << std::endl;

        }

        averages[sensorID] = averageVector;
        PQclear(resSensorData);
        
    }

    PQclear(resSensorsID);
    return true;

}