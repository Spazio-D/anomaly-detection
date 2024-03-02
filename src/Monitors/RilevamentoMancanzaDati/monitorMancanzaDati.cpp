#include <iostream>
#include <vector>
#include <string>
#include <map>
#include </opt/homebrew/opt/libpq/include/libpq-fe.h>

struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};

struct PeriodoMancante {
    std::string sensor;
    std::string periodoInizio;
    std::string periodoFine;
};


//Funzione per connettersi al database PostgreSQL
PGconn* connessioneAlDatabase() {
    const char *connString = "dbname=anomaly_detection user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432";
    PGconn *conn = PQconnectdb(connString);

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore durante la connessione al database PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }

    // // Query per la creazione della tabella se non esiste
    // const char *createTableQuery = "CREATE TABLE IF NOT EXISTS tabella_periodi_mancanti (id SERIAL PRIMARY KEY, sensor VARCHAR(255) NOT NULL, periodo_inizio VARCHAR(255) NOT NULL, periodo_fine VARCHAR(255) NOT NULL)";
    
    // PGresult *createTableResult = PQexec(conn, createTableQuery);
    // if (PQresultStatus(createTableResult) != PGRES_COMMAND_OK) {
    //     std::cerr << "Errore durante la creazione della tabella: " << PQerrorMessage(conn) << std::endl;
    //     PQclear(createTableResult);
    //     PQfinish(conn);
    //     return nullptr;
    // }
    // PQclear(createTableResult);

    std::cout << "Connessione al database PostgreSQL riuscita." << std::endl;
    return conn;
}

//Funzione per leggere i dati dalla tabella_dati
std::map<std::string, std::vector<Data>> leggiDatiDaDatabase(PGconn* conn) {
    std::map<std::string, std::vector<Data>> datiPersensor;

    const char *query = "SELECT sensor_id, sample_time, value FROM tabella_dati";
    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Errore durante l'esecuzione della query: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return datiPersensor;
    }

    int righe = PQntuples(result);

    for (int i = 0; i < righe; ++i) {
        Data dato;
        dato.sensorID = PQgetvalue(result, i, 0);
        dato.sampleTime = PQgetvalue(result, i, 1);
        dato.value = PQgetvalue(result, i, 2);

        //Organizza i dati nella mappa in base al sensor 
        datiPersensor[dato.sensorID].push_back(dato);
    }

    PQclear(result);

    return datiPersensor;
}

//Funzione per il monitor della mancanza di dati 
std::vector<PeriodoMancante> monitorMancanzaDati(const std::map<std::string, std::vector<Data>>& datiPersensor) {
    //Definisce una soglia per la mancanza di dati (può essere adattata)
    int sogliaMancanzaDati = 10; //Ad esempio, si considera un periodo senza dati se mancano almeno 3 campioni consecutivi.

    std::vector<PeriodoMancante> periodiMancanti;

    //Scorre ogni sensor nella mappa 
    for (const auto& coppia : datiPersensor) {
        const std::string& sensor = coppia.first;
        const std::vector<Data>& dati = coppia.second;

        //Verifica la mancanza di dati nella serie temporale
        int consecutiveMancanti = 0;
        std::string periodoInizio;
        bool inPeriodoMancante = false;

        for (const Data& dato : dati) {
            //Se il valore è 'NULL', incemente il conteggio di periodi consecutivi senza dati 
            if (dato.value == "NULL") {
                if (!inPeriodoMancante) {
                    //Memorizza il sampleTime all'inizio del periodo senza dati solo se non è già in un periodo mancante
                    periodoInizio = dato.sampleTime;
                    inPeriodoMancante = true;
                }
                consecutiveMancanti++;
            } else {
                //Se il valore è diverso da 'NULL', azzera il conreggio
                if (inPeriodoMancante) {
                    //Se sta uscendo da un periodo mancante, memorrizza le informazioni sul periodo mancante
                    if (consecutiveMancanti >= sogliaMancanzaDati) {
                        //std::cout << "Allarme: Mancanza di dati nel sensor " << sensor << " dal periodo " << periodoInizio << " al periodo " << dato.sampleTime << std::endl;
                        PeriodoMancante periodo;
                        periodo.sensor = sensor;
                        periodo.periodoInizio = periodoInizio;
                        periodo.periodoFine = dato.sampleTime;

                        //Aggiunge il periodo mancante al vettore
                        periodiMancanti.push_back(periodo);
                     }
                     inPeriodoMancante = false;
                }
                consecutiveMancanti = 0;
            }
        }
    }

    return periodiMancanti;
}

//Funzione per inserire i periodi mancanti nel database
void inserisciPeriodiMancantiInDatabase(PGconn* conn, const std::vector<PeriodoMancante>& periodiMancanti) {
    for (const PeriodoMancante& periodo : periodiMancanti) {
        std::string query = "INSERT INTO tabella_periodi_mancanti (sensor, periodo_inizio, periodo_fine) VALUES ('" + periodo.sensor + "', '" + periodo.periodoInizio + "', '" + periodo.periodoFine + "')";

        PGresult* result = PQexec(conn, query.c_str());

        if (PQresultStatus(result) != PGRES_COMMAND_OK) {
            std::cerr << "Errore durante l'inserimento dei dati dei periodi mancanti: " << PQerrorMessage(conn) << std::endl;
        }
        PQclear(result);
    }
}

int main() {
    //Connessione al database
    PGconn *conn = connessioneAlDatabase();
    if (!conn) {
        std::cerr << "impossibile connettersi al database. Uscita." << std::endl;
        return 1;
    }

    //Legge i dati dalla tabella_dati e organizzali in una mappa
    std::map<std::string, std::vector<Data>> datiPersensor = leggiDatiDaDatabase(conn);

    //Esegue il monitor per la rilevazione di mancanza di dati
    std::vector<PeriodoMancante> periodiMancanti = monitorMancanzaDati(datiPersensor);

    //Chiama funzione per salvare periodi mancanti in database
    inserisciPeriodiMancantiInDatabase(conn, periodiMancanti);

    return 0;
}