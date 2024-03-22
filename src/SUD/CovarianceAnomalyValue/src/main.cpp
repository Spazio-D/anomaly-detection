#include "main.h"

int main() {
    
    // Connessione al database
    PGconn *conn = PQconnectdb("dbname=anomalydetection user=ned password=47002 hostaddr=127.0.0.1 port=5432");
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore nella connessione a PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    // Lettura dati dal database e salvataggio in strutture dati
    std::map<std::string, std::vector<Data>> dataVector;
    std::map<std::string, std::vector<Average>> averages;
    std::vector<std::vector<std::vector<Covariance>>> covariances{};
    if(!readDataSQL(dataVector, averages, covariances, conn)){
        PQfinish(conn);
        return 1;
    }
    
    covariances.shrink_to_fit();
    
    // Calcolo del valore delle anomalie e salvataggio in SQL
    std::vector<std::vector<std::vector<AnomalyCovariance>>> covarianceAnomalyVector = calculateAnomaly(dataVector, averages, covariances);
    if(!saveAnomalySQL(covarianceAnomalyVector, conn)){
        PQfinish(conn);
        return 1;
    }

    PQfinish(conn);
    return 0;
}