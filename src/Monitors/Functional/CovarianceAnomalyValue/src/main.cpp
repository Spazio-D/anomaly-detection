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
    std::map<std::string, std::vector<Average>> averages;
    std::vector<std::vector<std::vector<Covariance>>> covariances{};
    if(!readDataSQL(dataVector, averages, covariances, conn)){
        PQfinish(conn);
        return 1;
    }

    covariances.shrink_to_fit();
    
    std::vector<std::vector<std::vector<AnomalyCovariance>>> covarianceAnomalyVector = calculateAnomaly(dataVector, averages, covariances);

    if(!saveAnomalySQL(covarianceAnomalyVector, conn)){
        PQfinish(conn);
        return 1;
    }

    PQfinish(conn);
    return 0;
}