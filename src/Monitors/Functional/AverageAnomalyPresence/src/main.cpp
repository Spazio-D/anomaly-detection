#include "main.h"

int main() {
    
    // Connessione al database
    PGconn *conn = PQconnectdb("dbname=anomalydetection user=ned password=47002 hostaddr=127.0.0.1 port=5432");
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore nella connessione a PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    // Lettura dei dati dal database e creazione e popolazione delle strutture dati per esse
    std::map<std::string, std::vector<Data>> dataVector;
    std::map<std::string, std::vector<Average>> averages;
    if(!readDataSQL(dataVector, averages, conn)){
        PQfinish(conn);
        return 1;
    }
    
    // Calcolo presenza delle anomalie
    detectAnomaly(dataVector, averages);
    
    // Salva i dati sul database SQL
    if(!saveAnomalySQL(averages, dataVector, conn)){
        PQfinish(conn);
        return 1;
    }
    
    PQfinish(conn);
    return 0;
}