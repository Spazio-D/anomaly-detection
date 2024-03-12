#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>
#include </opt/homebrew/opt/libpq/include/libpq-fe.h>

//Funzione per connettersi al database PostgreSQL
PGconn *connessioneAlDatabase() {
    const char *connString = "dbname=anomaly_detection user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432";
    PGconn *conn = PQconnectdb(connString);

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore durante la connessione al database PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }

    //std::cout << "Connessione al database PostgreSQL riuscita." << std::endl;
    return conn;
}

//Funzione per leggere la matrice di covarianza tra i sensors dal database
std::map<std::string, std::map<std::string, double>> leggiMatriceCovarianzaDalDatabase(PGconn *conn) {
    std::map<std::string, std::map<std::string, double>> matriceCovarianza;

    const char *query = "SELECT * FROM tabella_covarianze";
    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        std::cerr << "Errore durante l'esecuzione della query: " << PQerrorMessage(conn) << std::endl;
        PQclear(result);
        return matriceCovarianza;
    }

    int righe = PQntuples(result);

    if (righe == 0 ) {
        //std::cout << "Nessun dato trovato nella tabella_covarianze." << std::endl;
    } else {
        ////std::cout << "Matrice di covarianza tra i sensors:" << std::endl;
        for (int i = 0; i < righe; ++i) {
            std::string sensorID1 = PQgetvalue(result, i, 0);
            std::string sensorID2 = PQgetvalue(result, i, 1);
            const char *valoreStringa = PQgetvalue(result, i, 2);

            double valoreCovarianza = std::stod(valoreStringa);
            
            //Verifica se la chiave sensorID1 esiste nella mappa principale
            auto it1 = matriceCovarianza.find(sensorID1);
            if (it1 != matriceCovarianza.end()) {
                //Verifica se la chiave sensorID" esiste nella mappa interna 
                auto& innerMap = it1->second;
                auto it2 = innerMap.find(sensorID2);
                if (it2 != innerMap.end()) {
                    //Chiave trovata, sovrascrivi il valore
                    it2->second = valoreCovarianza;
                } else {
                    // Chiave non trovata, inserisci una nuova coppia
                    innerMap[sensorID2] = valoreCovarianza;
                }
            } else {
                //Chiave principale non trovata, crea una nuova mappa interna
                matriceCovarianza[sensorID1][sensorID2] = valoreCovarianza;
            }

            // //std::cout << sensorID1 << " " << sensorID2 << " " << valoreCovarianza << std::endl;
        }
    }
    PQclear(result);

    return matriceCovarianza;
}

// Funzione per rilevare covarianze anomale
std::map<std::string, std::map<std::string, double>>  rilevaAnomalieCovarianza(const std::map<std::string, std::map<std::string, double>>& matriceCovarianza) {
    std::map<std::string, std::map<std::string, double>> sensorsAnomali;

    //Definisce una soglia per le covarianze anomale (si può adattare questo valore)
    double sogliaAnomalie = 1e-5;

    //Rileva e segnala le covarianze anomale
    ////std::cout << "Covarianze anomale rilevate: " << std::endl;

    for (const auto& coppia1 : matriceCovarianza) {
        const std::string& sensorID1 = coppia1.first;

        for (const auto& coppia2 : coppia1.second) {
            const std::string& sensorID2 = coppia2.first;
            double covarianza = coppia2.second;

            //Si può addattare questa condizione in base alle esigenze 
            if (std::abs(covarianza) > sogliaAnomalie) {
                // //std::cout << "Coppia di sensors: " << sensorID1 << " e " << sensorID2 << ", Covarianza: " << covarianza << std::endl;

                //Aggiungi i sensors alla mappa dei sensors anomali
                sensorsAnomali[sensorID1][sensorID2] = covarianza;
            }
        }
    }
    return sensorsAnomali;
}

//Funzione per salvare i sensors anomali nel database PostgreSQL
void salvasensorsAnomaliInPostgreSQL(PGconn *conn,  const std::map<std::string, std::map<std::string, double>>& sensorsAnomali) {
    //Inizia una transazione
    PGresult *res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Errore durante l'inizio della transazione: " << PQresultErrorMessage(res) << std:: endl;
        PQclear(res);
        return;
    }
    PQclear(res);

    //Itera sulla mappa e inserisce i dati nel database
    for (const auto& coppia1 : sensorsAnomali) {
        const std::string& sensorID1 = coppia1.first;

        for (const auto& coppia2 : coppia1.second) {
            const std::string& sensorID2 = coppia2.first;
            double anomalia = coppia2.second;

            //Costruisci la query di inserimento
            std::string query = "INSERT INTO tabella_anomalie_covarianza (sensor_id1, sensor_id2, anomalia) VALUES ('" + sensorID1 + "', '" + sensorID2 + "', " + std::to_string(anomalia) + ")";

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

    //Consludi la transazione
    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Errore durante la conclusione della transazione: " << PQresultErrorMessage(res) << std::endl;
    }
    PQclear(res);
}

int main() {
    //Connessione al database
    PGconn *conn = connessioneAlDatabase();
    if (!conn) {
        std::cerr << "Impossibile connetersi al database. Uscita." << std::endl;
        return 1;
    }

    //Legge la matrice di covarianza tra i sensors al database
    std::map<std::string, std::map<std::string, double>> matriceCovarianza = leggiMatriceCovarianzaDalDatabase(conn);

    //Chaimo funzione di anomalie
    std::map<std::string, std::map<std::string, double>> sensorsAnomali = rilevaAnomalieCovarianza(matriceCovarianza);

    salvasensorsAnomaliInPostgreSQL(conn, sensorsAnomali);

    //Chiudi la connessione al database
    PQfinish(conn);

    return 0;
}