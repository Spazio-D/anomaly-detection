#include "main.h"

//Funzione per salvare le covarianze nel database PostgreSQL
void salvaCovarianzeInPostgreSQL(const std::map<std::string, std::map<std::string, double>>& matriceCovarianza) {
    //Crea una conessione al databse 
    PGconn *conn = PQconnectdb("dbname=anomaly_detection user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432");

    //Controlla se la connessione è avvenuta con sucesso
    if (PQstatus(conn) == CONNECTION_OK) {
        std::cout << "Connessione al database PostgreSQL aperta con successo." << std::endl;

        //Inizia una transazione
        PGresult *res = PQexec(conn, "BEGIN");
        if (PQresultStatus(res) == PGRES_COMMAND_OK) {
            PQclear(res);

            //Inserisce le covarianze nel database
            for (const auto& coppia1 : matriceCovarianza) {
                const std::string& sensoreID1 = coppia1.first;

                for (const auto& coppia2 : coppia1.second) {
                    const std::string& sensoreID2 = coppia2.first;
                    double covarianza = coppia2.second;

                    //Costuisce la query di inserimento
                    std::string query = "INSERT INTO tabella_covarianze (sensore_id1, sensore_id2, covarianza) VALUES ('" + sensoreID1 + "', '" + sensoreID2 + "', '" + std::to_string(covarianza) + "')";

                    //Esegue la query
                    res = PQexec(conn, query.c_str());
                    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                        std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) <<std::endl;
                        PQclear(res);
                        break;
                    }

                    PQclear(res);
                }

            }

            //Conclude la transazione
            res = PQexec(conn, "COMMIT");
            PQclear(res);
        } else {
            std::cerr << "Errore durante l'inizio della transazione: " << PQresultErrorMessage(res) << std::endl;
            PQclear(res);
        }

        //Chiude la connessione
        PQfinish(conn);
    } else {
        std::cerr << "Errore durante l'apertura della connessione al database PostgreSQL." << std::endl;
    }
}