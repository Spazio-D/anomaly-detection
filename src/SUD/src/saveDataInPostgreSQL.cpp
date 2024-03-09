#include "main.h"

bool saveDataInPostgreSQL(std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn){

    PGresult *res;    

    for (auto element : dataVector) {
        for (Data data : dataVector[element.first]) {

            std::string query = "INSERT INTO dataTable (sensorID, sampleTime, value) VALUES ('" + data.sensorID + "', " + data.sampleTime + ", " + data.value + ")";
            
            res = PQexec(conn, query.c_str());
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
