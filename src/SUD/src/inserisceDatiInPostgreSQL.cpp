#include "main.h"

//Funzione per inserire dati in PostgreSQL
void inserisciDatiInPostgreSQL(const std::map<std::string, std::vector<Data>>& datiRaggrupati) {
    //Crea una conessione al databse 
    PGconn *conn = PQconnectdb("dbname=dati_di_sensors user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432");

    //Controlla se la connessione è avvenuta con sucesso
    if (PQstatus(conn) == CONNECTION_OK) {
        std::cout << "Connessione al database PostgreSQL aperta con successo." << std::endl;

        //Inizia una transazione
        PGresult *res = PQexec(conn, "BEGIN");
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'inizio della transazione: " << PQresultErrorMessage(res) << std::endl;
            PQclear(res);
            return;
        }
        PQclear(res);

        //inserisce i dati nel database
        for (const auto& coppia : datiRaggrupati) {
            // const std::string& sensorID = coppia.first;
            // const std::vector<Data>& dati = coppia.second;

            for (const Data& dato : coppia.second) {
                //Costruisci la query di inserimento
                std::string query = "INSERT INTO tabella_dati (sensor_id, sample_time, value) VALUES ('" + dato.sensorID + "', '" + dato.sampleTime + "', '" + dato.value + "')";

                //Esegui la query
                res = PQexec(conn, query.c_str());
                if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                    std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
                    PQclear(res);
                    break;
                }

                PQclear(res);
            }
        }

        //Concludi la transazione
        res = PQexec(conn, "COMMIT");
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante la conclusione della transazione: " << PQresultErrorMessage(res) << std::endl;
        }
        PQclear(res);
    }
}