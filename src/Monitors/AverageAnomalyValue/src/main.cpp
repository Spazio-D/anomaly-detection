#include "main.h"

int main() {
    
    PGconn *conn = PQconnectdb("dbname=anomalydetection user=ned password=47002 hostaddr=127.0.0.1 port=5432");
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore nella connessione a PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    // Read data from the database
    std::map<std::string, std::vector<Data>> dataVector;
    std::map<std::string, double> averages;
    if(!readDataSQL(dataVector, averages, conn)){
        return 1;
    }

    
    





// standard deviation

// calculate the presence of anomalies

// write the results to the database

}