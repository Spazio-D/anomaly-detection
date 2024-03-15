#include "main.h"

bool updateDataSQL(PGconn *conn){

    std::string query = "SELECT sensorID, firstsampletime, nullStreak FROM missingDataTable;";
    PGresult *resSelect = PQexec(conn, query.c_str());
    if (PQresultStatus(resSelect) != PGRES_TUPLES_OK) {
        std::cerr << "Errore nella query di selezione della nullStreak: " << PQerrorMessage(conn) << std::endl;
        PQclear(resSelect);
        return false;
    }

    int rows = PQntuples(resSelect);
    for (int i = 0; i < rows; i++) {
        std::string sensorID = PQgetvalue(resSelect, i, 0);
        std::string firstsampletime = PQgetvalue(resSelect, i, 1);
        std::string nullStreak = PQgetvalue(resSelect, i, 2);
        std::string isAnomaly = std::stoi(nullStreak) > MAXNULLDATASTREAK ? "TRUE" : "FALSE";

        query = "UPDATE missingDataTable SET isAnomaly = " + isAnomaly + " WHERE sensorID = '" + sensorID + "' AND firstSampleTime = " + firstsampletime + ";";
        PGresult *resUpdate = PQexec(conn, query.c_str());
        if (PQresultStatus(resUpdate) != PGRES_COMMAND_OK) {
            std::cerr << "Errore nell'aggiornamento della nullStreak: " << PQerrorMessage(conn) << std::endl;
            PQclear(resUpdate);
            return false;
        }

        PQclear(resUpdate);
    }

    PQclear(resSelect);
    return true;
}