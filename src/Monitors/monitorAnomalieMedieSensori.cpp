#include <iostream>
#include <cmath>
#include <map>
#include </opt/homebrew/opt/libpq/include/libpq-fe.h>

//Funzione per connettersi al database PostgreSQL
PGconn *connessioneAlDatabase() {
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

//Funzione per leggere i dati storici delle medie dei sensori dal database
std::map<std::string, double> leggiDatiMedieDalDatabase(PGconn *conn) {
    std::map<std::string, double> medie;

    const char *query = "SELECT * FROM tabella_medie";
    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Errore durante l'esecuzione della query: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return medie;
    }

    int righe = PQntuples(result);
    if (righe == 0) {
        std::cout << "Nessun dato trovato nella tabella_medie." << std::endl;
    } else {
        //std::cout << "Dati storici delle medie dei sensori:" << std::endl;
        for (int i = 0; i < righe; ++i) {
            std::string sensorID = PQgetvalue(result, i, 0);
            double valoreMedio = std::stod(PQgetvalue(result, i, 1));
            medie[sensorID] = valoreMedio;

            //std::cout << "Sensore ID: " << sensorID << ", Valore Medio: " << valoreMedio << std::endl;
        }
    }
    PQclear(result);

    return medie;
}

//Funzione per rilevare anomalie sulla deviazione standard
std::map<std::string, double> rilevaAnomalieMedie(const std::map<std::string, double>& medie) {
    std::map<std::string, double> sensoriAnomali;

    //Calcola la media globale 
    double mediaGlobale = 0.0;
    for (const auto& coppia : medie) {
        mediaGlobale += coppia.second;
    }
    mediaGlobale /= medie.size(); 

    //std::cout << "Media globale: " << mediaGlobale << std::endl;

    //Calcola la deviazione standard globale
    double deviazioneStandardGlobale = 0.0;
    for (const auto& coppia : medie) {
        deviazioneStandardGlobale += std::pow(coppia.second - mediaGlobale, 2);
    }
    deviazioneStandardGlobale = std::sqrt(deviazioneStandardGlobale / medie.size());

    //std::cout << "Deviazione Standard Globale: " << deviazioneStandardGlobale << std::endl;

    //Definisce una soglia per le anomalie (ad esempio, 2 deviazioni standard)
    double sogliaAnomalie = 2.0 * deviazioneStandardGlobale;

    //std::cout << "Soglia per anomalie: " << sogliaAnomalie << std::endl;

    //Rileva e segnala le anomalie
    //std::cout << "Anomalie rilevate: " << std::endl;
    for (const auto& coppia : medie) {
        if (std::abs(coppia.second - mediaGlobale) > sogliaAnomalie) {
            //std::cout << "Sensore: " << coppia.first << ", Valore Medio: " << coppia.second << std::endl;

            //Aggiunge il sensore anomalo alla mappa
            sensoriAnomali[coppia.first] = coppia.second;
        }
    }
    return sensoriAnomali;
}

//Funzione per inviare le informazioni sulle anomali al database
void inviaAnomalieAlDatabase(PGconn *conn, const std::map<std::string, double>& sensoriAnomali) {
    for (const auto& coppia : sensoriAnomali) {
        std::string query = "INSERT INTO tabella_anomalie_medie (sensorID, valoreAnomalo) VALUES ('" + coppia.first + "', " + std::to_string(coppia.second) + ")";
        PGresult *result = PQexec(conn, query.c_str());

        if (PQresultStatus(result) != PGRES_COMMAND_OK) {
            std::cerr << "errore durante l'inserimento dei dati sulle anomalie: " << PQerrorMessage(conn) << std::endl;
        } //else {
        //     std::cout << "Dati sul sensore anomalo inseriti corretamente nel database." << std::endl;
        // }

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

    //Legge i dati storici delle medie dei sensori dal database
    std::map<std::string, double> medie = leggiDatiMedieDalDatabase(conn);

    //Rileva anomalie rispetto al modello previsto
    std::map<std::string, double> sensoriAnomali = rilevaAnomalieMedie(medie);

    //Invia sensori con anomalie al database
    inviaAnomalieAlDatabase(conn, sensoriAnomali);

    // //Stampa le anomalie
    // std::cout << "Anomalie rilevate: " << std::endl;
    // for (const auto& coppia : sensoriAnomali) {
    //     std::cout << "Sensore: " << coppia.first << ", Valore Medio: " << coppia.second << std::endl;
    // }

    //Chiude la connessione al database
    PQfinish(conn);

    return 0;
}