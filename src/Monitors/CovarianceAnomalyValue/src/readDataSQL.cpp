#include "main.h"

bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::vector<std::vector<Covariance>> &covariances, PGconn *conn){

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
    std::vector<std::string> sensors;
    for (int i = 0; i <PQntuples(resSensorsID); i++) {

        sensorID = PQgetvalue(resSensorsID, i, 0);
        sensors.push_back(sensorID);
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

    }

    covariances.resize(sensors.size(), std::vector<Covariance>(sensors.size()));
    std::string sensor1;
    std::string sensor2;
    Covariance covariance;
    std::string check;
    for(size_t i = 0; i<dataVector.size(); i++){
        
        sensor1 = sensors[i];

        for(size_t j = 0; j<dataVector.size(); j++){

            sensor2 = sensors[j];
            query = "SELECT value, firstSampleTime, lastSampleTime FROM covarianceTable WHERE sensorID1 = '" + sensor1 + "' AND sensorID2 = '" + sensor2 + "'";
            resSensorData = PQexec(conn, query.c_str());

            if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
                std::cerr << "Errore nell'esecuzione della select su covarianceTable del sensorID1 " + sensor1 + " e sensorID2 " + sensor2 + ": " << PQresultErrorMessage(resSensorData) << std::endl;
                PQclear(resSensorData);
                PQclear(resSensorsID);
                PQfinish(conn);
                return false;
            }
      
            for(int k = 0; k<PQntuples(resSensorData); k++){

                covariance.sensorID1 = sensor1;
                covariance.sensorID2 = sensor2;
                check = PQgetvalue(resSensorData, k, 0);
                if(check == ""){
                    covariance.value = std::nan("");
                }else{
                    covariance.value = std::stod(check);
                }
                covariance.firstSampleTime = std::stoi(PQgetvalue(resSensorData, k, 1));
                covariance.lastSampleTime = std::stoi(PQgetvalue(resSensorData, k, 2));
                covariances[i][j] = covariance;
                //std::cout << "covariances[" << i << "][" << j << "]: " << covariances[i][j].sensorID1 << " " << covariances[i][j].sensorID2 << " " << covariances[i][j].value << " " << covariances[i][j].firstSampleTime << " " << covariances[i][j].lastSampleTime << std::endl;

            }

            PQclear(resSensorData);
            
        }
        
        

    }

    PQclear(resSensorsID);
    PQfinish(conn);
    return true;

}