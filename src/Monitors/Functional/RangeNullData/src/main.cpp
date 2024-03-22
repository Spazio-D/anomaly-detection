#include "main.h"

int main() {
    
    // Connessione al database
    PGconn *conn = PQconnectdb("dbname=anomalydetection user=ned password=47002 hostaddr=127.0.0.1 port=5432");
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore nella connessione a PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    // Aggiornamento dei dati nel database
    if(!updateDataSQL(conn)){
        PQfinish(conn);
        return 1;
    }
    
    PQfinish(conn);
    return 0;
}