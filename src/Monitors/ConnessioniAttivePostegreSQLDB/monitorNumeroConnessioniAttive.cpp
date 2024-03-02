#include <iostream>
#include </opt/homebrew/opt/libpq/include/libpq-fe.h>

//Funzione per connettersi al database PostgreSQL
PGconn* connessioneAlDatabase() {
    const char *connString = "dbname=dati_di_sensors user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432";
    PGconn *conn = PQconnectdb(connString);

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore durante la connessione al datbase PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }

    std::cout << "Connessione al database PostgreSQL riuscita." << std::endl;
    return conn;
}

//Funzione per il monitor del numero di connessioni attive al database
void monitorNumeroConnessioniAttive(PGconn* conn) {
    //Definizione di una soglia per il numero di connessioni attive (adattabile alle esigenze)
    int sogliaConnessioniAttive = 0; //Ad esempio, si considera critico se ci sono più di 10 connessioni attive

    //Esecuzione della query per ottenere il numerp di connessioni attive
    const char* query = "SELECT count(*) FROM pg_stat_activity WHERE state = 'active'";
    PGresult* result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Errore durante l'esecuzione della query per il monitor del numero di connessioni attive: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return;
    }

    //Recupero del risultato della query 
    int numeroConnessioniAttive = std::stoi(PQgetvalue(result, 0, 0));

    //Valutazione e segnalazione di un aumento significativo delle connessioni
    if (numeroConnessioniAttive > sogliaConnessioniAttive) {
        std::cout << "Allarme: Aumento significativo delle connessioni attive al database. Numero attuale: " << numeroConnessioniAttive << std::endl;

        //Inserimento dei dati nella tabella di monitoraggio
        std::string numeroConnessioniString = std::to_string(numeroConnessioniAttive);
        const char* insertQuery = "INSERT INTO tabella_monitoraggio_connessioni_attive (numero_connessioni, timestamp) VALUES ($1, CURRENT_TIMESTAMP)";
        const char* paramValues[1] = { numeroConnessioniString.c_str() };
        PGresult* insertResult = PQexecParams(conn, insertQuery, 1, nullptr, paramValues, nullptr, nullptr, 0);

        if (PQresultStatus(insertResult) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'inserimento dei dati nella tabella di monitoraggio: " << PQerrorMessage(conn) << std::endl;
        }

        PQclear(insertResult);
     } else {
        std::cout << "Numero attuale di connessione attive: " << numeroConnessioniAttive << std::endl;
    }

    PQclear(result);
}

int main() {
    //Connessione al databse
    PGconn *conn = connessioneAlDatabase();
    if (!conn) {
        std::cerr << "Impossibile connetersi al databse. Uscita." << std::endl;
        return 1;
    }

    //Esecuzione del monitor per il numero di connessioni attive 
    monitorNumeroConnessioniAttive(conn);

    //Chiusura della connessione
    PQfinish(conn);

    return 0;
}