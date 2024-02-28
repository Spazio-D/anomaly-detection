#include <iostream>
#include <vector>
#include <string>
#include </opt/homebrew/opt/libpq/include/libpq-fe.h>

struct QueryTempoRisposta {
    std::string query;
    double tempoRisposta;
};

//Funzione per connettersi al database PostgreSQL
PGconn* connessioneAlDatabase() {
    const char *connString = "dbname=dati_di_sensori user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432";
    PGconn *conn = PQconnectdb(connString);

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore durante la connessione al database PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }

    std::cout << "Connessione al database PostgreSQL riuscita." << std::endl;
    return conn;
}

//Funzione per inserire i risultati del monitoraggio nel databse
void inserisciRisultatoMonitoraggio(PGconn* conn, const std::string& query, double tempoRisposta) {
    std::string queryInserimento = "INSERT INTO tabella_monitoraggio_tempo_risposta (query_text, tempo_risposta) VALUES ('" + query + "', " + std::to_string(tempoRisposta) + ")";

    PGresult* result = PQexec(conn, queryInserimento.c_str());

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        std::cerr << "Errore durante l'inserimento dei dati del monitoraggio del tempo di risposta: " << PQerrorMessage(conn) << std::endl;
    }

    PQclear(result);
}

//Funzione per il monitor del tempo di risposta del database
void monitorTempoRispostaDatabase(PGconn* conn) {
    std::vector<QueryTempoRisposta> queryTempoRisposte;

    //Definisce soglia per il tempo di risposta del database (adattabile alle esigenze)
    double sogliaTempoRispostaAlta = 50.0; //Ad esempio, considera tempi superiori a 50 ms come elevati

    //Esegue una query per ottenere i tempi di risposta del database
    const char* query = "SELECT query, total_exec_time FROM pg_stat_statements";
    PGresult* result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Errore durante l'esecuzione della query per il monitor del tempo di risposta del database: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return;
    }

    int righe = PQntuples(result);

    //Scansiona i risultati della query e memorizza i dati in una struttura
    for (int i = 0; i < righe; ++i) {
        QueryTempoRisposta queryTempoRisposta;
        queryTempoRisposta.query = PQgetvalue(result, i, 0);
        queryTempoRisposta.tempoRisposta = std::stod(PQgetvalue(result, i, 1));

        queryTempoRisposte.push_back(queryTempoRisposta);
    }

    PQclear(result);

    //Valuta e segnala deviazioni significative nel tempi di risposta
    for (const QueryTempoRisposta& qr : queryTempoRisposte) {
        if (qr.tempoRisposta > sogliaTempoRispostaAlta) {
            std::cout << "Avviso: Tempo di risposta elevato per la query '" << qr.query << "' con tempo " << qr.tempoRisposta << " ms" << std::endl;

            //Inserisci i risultati nel database
            inserisciRisultatoMonitoraggio(conn, qr.query, qr.tempoRisposta);
        }
    }
}

int main() {
    //Connessione al database
    PGconn *conn = connessioneAlDatabase();
    if (!conn) {
        std::cerr << "Impossibile connettersi al database. Uscita." << std::endl;
        return 1;
    }

    //Esegue il monitor per il tempo di risposta del database
    monitorTempoRispostaDatabase(conn);

    //Chiude la connessione
    PQfinish(conn);

    return 0;
}