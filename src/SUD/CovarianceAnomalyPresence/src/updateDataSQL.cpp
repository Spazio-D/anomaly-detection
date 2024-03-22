#include "main.h"

bool updateDataSQL(PGconn *conn){
    
    // Query per la selezione dei dati dalla tabella delle anomalie delle covarianze
    std::string query = "SELECT * FROM anomalyCovarianceTable;";
    PGresult *resSelect = PQexec(conn, query.c_str());
    if (PQresultStatus(resSelect) != PGRES_TUPLES_OK || PQntuples(resSelect) == 0) {
        std::cerr << "Errore durante l'esecuzione della query di selezione anomalie di covarianza: " << PQresultErrorMessage(resSelect) << std::endl;
        PQclear(resSelect);
        return false;
    }

    // Scorrimento dei valori della tabella delle anomalie delle covarianze
    int nRows = PQntuples(resSelect);
    for(int i = 0; i < nRows; i++){

        std::string sensorID1 = PQgetvalue(resSelect, i, 0);
        std::string sensorID2 = PQgetvalue(resSelect, i, 1);
        std::string sampleTime = PQgetvalue(resSelect, i, 2);
        std::string value = PQgetvalue(resSelect, i, 4);

        // Controllo presenza dell'anomalia
        std::string isAnomaly;
        if(value == ""){
            isAnomaly = "FALSE";
        }else{
            isAnomaly = detectAnomaly(std::stod(value)) ? "FALSE" : "TRUE";
        }

        // Query per l'aggiornamento della tabella delle anomalie delle covarianze
        query = "UPDATE anomalyCovarianceTable SET isAnomaly = " + isAnomaly + ", detectionTime = CURRENT_TIME WHERE sensorID1 = '" + sensorID1 + "' AND sensorID2 = '" + sensorID2 + "' AND firstSampleTime = " + sampleTime + ";";
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