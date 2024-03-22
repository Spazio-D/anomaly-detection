#include "main.h"

bool saveDataInPostgreSQL(std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn){
    
    // Scorrimento dei dati dei sensori
    for (auto element : dataVector) {
        for (Data data : dataVector[element.first]) {

            // Creazione query e inserimento dati nel Database
            std::string query = "INSERT INTO dataTable (sensorID, sampleTime, value) VALUES ('" 
                                + data.sensorID + "', " + data.sampleTime + ", " + data.value + ")";
            PGresult *res = PQexec(conn, query.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
                PQclear(res);
                return false;
            }

            PQclear(res);
        }
    }

    return true;
}
