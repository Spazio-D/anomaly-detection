#include "main.h"

bool saveAnomalySQL(std::map<std::string, std::vector<Data>> &dataVector, int windowSize, PGconn *conn){
    
    // Scorrimento dei dati dei sensori
    for (auto element : dataVector){
        for (Data data : dataVector[element.first]){

            // Query per l'aggiornamento della tabella delle anomalie delle medie
            std::string query = "UPDATE anomalyAverageTable SET value = " + std::to_string(data.averageAnomalyValue) + " WHERE sensorID = '" + data.sensorID + "' AND firstSampleTime = " + std::to_string(std::stoi(data.sampleTime) - windowSize+1);
            PGresult *res = PQexec(conn, query.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::cerr << "Errore durante l'esecuzione della query di inserimento anomalie di media: " << PQresultErrorMessage(res) << std::endl;
                PQclear(res);
                return false;
            }
            
            PQclear(res);
        }
    }
    return true;
}
