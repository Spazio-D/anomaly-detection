#include "main.h"

bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, std::vector<std::vector<std::vector<Covariance>>> &covariances, PGconn *conn){

    // Query per selezione dei sensorID 
    std::string query = "SELECT DISTINCT sensorID FROM datatable sensorID;";
    PGresult *resSensorsID = PQexec(conn, query.c_str());
    if (PQresultStatus(resSensorsID) != PGRES_TUPLES_OK) {
        std::cerr << "Errore nell'esecuzione della select su dataTable dei sensorID: " << PQresultErrorMessage(resSensorsID) << std::endl;
        PQclear(resSensorsID);
        return false;
    }

    // Scorrimento dei sensorID e creazione e popolazione di un vettore di essi
    std::vector<std::string> sensors;
    for (int i = 0; i <PQntuples(resSensorsID); i++) {

        std::string sensorID = PQgetvalue(resSensorsID, i, 0);
        sensors.push_back(sensorID);

        // Query per selezione dei dati dello specifico sensorID  
        query = "SELECT sampleTime, value FROM dataTable WHERE sensorID = '" + sensorID + "'";
        PGresult *resSensorData = PQexec(conn, query.c_str());
        if (PQresultStatus(resSensorData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su dataTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resSensorData) << std::endl;
            PQclear(resSensorData);
            PQclear(resSensorsID);
            return false;
        }

        // Scorrimento dei dati del sensore e popolazione struttura dati per essi
        for (int j = 0; j <PQntuples(resSensorData); j++) {

            Data data;
            data.sensorID = sensorID;
            data.sampleTime = PQgetvalue(resSensorData, j, 0);
            data.value = PQgetvalue(resSensorData, j, 1);
            dataVector[sensorID].push_back(data);

        }

        PQclear(resSensorData);

        // Query per selezione del valore della media del sensore e della finestra temporale
        query = "SELECT value, firstSampleTime, lastSampleTime FROM averageTable WHERE sensorID = '" + sensorID + "'";
        PGresult *resAverageData = PQexec(conn, query.c_str());
        if (PQresultStatus(resAverageData) != PGRES_TUPLES_OK) {
            std::cerr << "Errore nell'esecuzione della select su averageTable del sensorID " + sensorID + ": " << PQresultErrorMessage(resAverageData) << std::endl;
            PQclear(resAverageData);
            PQclear(resSensorsID);
            return false;
        }

        // Scorrimento delle medie del sensore per finestra temporarle e creazione e popolazione della struttura dati per ese
        std::vector<Average> averageVector;
        for(int j = 0; j < PQntuples(resAverageData); j++){

            Average average;
            std::string averageValue;
            average.sensorID = sensorID;
            averageValue = PQgetvalue(resAverageData, j, 0);
            if(averageValue == ""){
                average.value = std::nan("");
            }else{
                average.value = std::stod(averageValue);
            }
            
            average.firstSampleTime = std::stoi(PQgetvalue(resAverageData, j, 1));
            average.lastSampleTime = std::stoi(PQgetvalue(resAverageData, j, 2));
            averageVector.push_back(average);

        }

        averages[sensorID] = averageVector;
        PQclear(resAverageData);
    }

    // Ordinamento del vettore di sensorID
    std::sort(sensors.begin(), sensors.end(), sensorSorting);

    //Scorrimento del vettore di dati dei sensori
    for(size_t i = 0; i<dataVector.size(); i++){
        
        covariances.emplace_back();
        std::string sensor1 = sensors[i];

        for(size_t j = 0; j<dataVector.size(); j++){
            
            covariances[i].emplace_back();
            std::string sensor2 = sensors[j];

            // Query per selezione del valore della covarianza dati i sensorID
            query = "SELECT value, firstSampleTime, lastSampleTime FROM covarianceTable WHERE sensorID1 = '" + sensor1 + "' AND sensorID2 = '" + sensor2 + "'";
            PGresult *resCovarianceData = PQexec(conn, query.c_str());
            if (PQresultStatus(resCovarianceData) != PGRES_TUPLES_OK) {
                std::cerr << "Errore nell'esecuzione della select su covarianceTable del sensorID1 " + sensor1 + " e sensorID2 " + sensor2 + ": " << PQresultErrorMessage(resCovarianceData) << std::endl;
                PQclear(resCovarianceData);
                PQclear(resSensorsID);
                return false;
            }

            // Scorrimento delle covarianze e creazione e popolazione del vettore di esse
            std::vector<Covariance> covarianceVector;
            for(int k = 0; k<PQntuples(resCovarianceData); k++){

                Covariance covariance;
                covariance.sensorID1 = sensor1;
                covariance.sensorID2 = sensor2;
                std::string covarianceValue = PQgetvalue(resCovarianceData, k, 0);
                if(covarianceValue == ""){
                    covariance.value = std::nan("");
                }else{
                    covariance.value = std::stod(covarianceValue);
                }
                covariance.firstSampleTime = std::stoi(PQgetvalue(resCovarianceData, k, 1));
                covariance.lastSampleTime = std::stoi(PQgetvalue(resCovarianceData, k, 2));
                covarianceVector.push_back(covariance);
            }
            
            covariances[i][j] = covarianceVector;
            PQclear(resCovarianceData);
        }
    }

    PQclear(resSensorsID);
    return true;
}