#include "main.h"

bool updateDataSQL(PGconn *conn){
    
    std::string query = "SELECT * FROM anomalyCovarianceTable;";
    PGresult *resSelect = PQexec(conn, query.c_str());

    if (PQresultStatus(resSelect) != PGRES_TUPLES_OK || PQntuples(resSelect) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione anomalie di covarianza: " << PQresultErrorMessage(resSelect) << std::endl;
        PQclear(resSelect);
        return false;
    }

    std::string sensorID1;
    std::string sensorID2;
    std::string sampleTime;
    std::string value;
    std::string isAnomaly;
    int nRows = PQntuples(resSelect);
    for(int i = 0; i < nRows; i++){
        sensorID1 = PQgetvalue(resSelect, i, 0);
        sensorID2 = PQgetvalue(resSelect, i, 1);
        sampleTime = PQgetvalue(resSelect, i, 2);
        value = PQgetvalue(resSelect, i, 4);

        if(value == ""){
            isAnomaly = "FALSE";
        }else{
            isAnomaly = detectAnomaly(std::stod(value)) ? "FALSE" : "TRUE";
        }

        query = "UPDATE anomalyCovarianceTable SET isAnomaly = " + isAnomaly + " WHERE sensorID1 = '" + sensorID1 + "' AND sensorID2 = '" + sensorID2 + "' AND firstSampleTime = " + sampleTime + ";";
        PGresult *resUpdate = PQexec(conn, query.c_str());
        if (PQresultStatus(resUpdate) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'esecuzione della query di aggiornamento anomalie di covarianza: " << PQresultErrorMessage(resUpdate) << std::endl;
            PQclear(resUpdate);
            return false;
        }

        PQclear(resUpdate);
    }

    PQclear(resSelect);
    return true;

}