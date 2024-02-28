#include "main.h"

// //Funzione per salvare le medie nel database PostgreSQL con libreria libpq-dev
// void salvaMedieInPostgreSQL(const std::map<std::string, double>& medie) {
//     //Crea una connesione al database
//     PGconn *conn = PQconnectdb("dbname=anomaly_detection user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432");

//     //Controlla se la connessione è avvenuta con successo 
//     if (PQstatus(conn) == CONNECTION_OK) {
//         std::cout << "Connessione al database PostgreSQL aperta con successo." << std::endl;

//         //Inizia una transazione 
//         PGresult *res = PQexec(conn, "BEGIN");
//         if (PQresultStatus(res) == PGRES_COMMAND_OK) {
//             PQclear(res);

//             //Inserisce le medie nel databse
//             for (const auto& media : medie) {
//                 std::string sensoreID = media.first;
//                 double valoreMedio = media.second;

//                 //Costuisce la query di inserimento
//                 std::ostringstream queryStream;
//                 queryStream << std::string("INSERT INTO tabella_medie (sensore_id, valore_medio) VALUES ('") << sensoreID << "', '" << valoreMedio << "')";
//                 std::string query = queryStream.str();
//                 //deb
//                 //std::cout << "Query SQL: " << query << std::endl;

//                 //Esegue la query
//                 res = PQexec(conn, query.c_str());
//                 if (PQresultStatus(res) != PGRES_COMMAND_OK) {
//                     std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
//                     PQclear(res);
//                     break;
//                 }

//                 PQclear(res);

//                 //deb
//                 //std::cout << "Dati: Sensore ID=" << sensoreID << ", Valore Medio=" << valoreMedio << std::endl;
//             }

//             //Conclude la transazione 
//             res = PQexec(conn, "COMMIT");
//             PQclear(res);
//         }else {
//             std::cerr << "Errore durante l'inizio della transazione: " << PQresultErrorMessage(res) << std::endl;
//             PQclear(res);
//         }

//         //Chiude la connessione 
//         PQfinish(conn);
//     } else {
//         std:: cerr << "Errore dutante l'apertura della connessione al database PostgreSQL." << std::endl;
//     }
// }

//Funzione per salvare le medie nel database PostgreSQL con libreria libpq-dev
void salvaMedieInPostgreSQL(const std::map<std::string, std::vector<std::tuple<int, int, double>>>& mediePerSensore) {
    //Crea una connesione al database
    PGconn *conn = PQconnectdb("dbname=dati_di_sensori user=Nastya password=nastyasql hostaddr=127.0.0.1 port=5432");

    //Controlla se la connessione è avvenuta con successo 
    if (PQstatus(conn) == CONNECTION_OK) {
        std::cout << "Connessione al database PostgreSQL aperta con successo." << std::endl;

        //Inizia una transazione 
        PGresult *res = PQexec(conn, "BEGIN");
        if (PQresultStatus(res) == PGRES_COMMAND_OK) {
            PQclear(res);

            //Inserisce le medie nel databse
            for (const auto& coppia : mediePerSensore) {
                const std::string& sensoreID = coppia.first;
                const std::vector<std::tuple<int, int, double>>& mediePerFinestra = coppia.second;

                for (const auto& tupla : mediePerFinestra) {
                    int startSampleTime = std::get<0>(tupla);
                    int endSampleTime = std::get<1>(tupla);
                    double media = std::get<2>(tupla);

                    //Costuisce la query di inserimento
                    std::string query = "INSERT INTO tabella_medie (sensore_id, start_sample_time, end_sample_time, valore_medio) VALUES ('" + sensoreID + "', " + std::to_string(startSampleTime) + ", " + std::to_string(endSampleTime) + ", " + std::to_string(media) + ")";

                    // Debug: stampa i valori prima di eseguire la query
                    //std::cout << "Query SQL: " << query << std::endl;

                    //Esegue la query
                    res = PQexec(conn, query.c_str());
                    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                        std::cerr << "Errore durante l'esecuzione della query: " << PQresultErrorMessage(res) << std::endl;
                        PQclear(res);
                        break;
                    }

                    PQclear(res);
                }
            }

            //Conclude la transazione 
            res = PQexec(conn, "COMMIT");
            PQclear(res);
        }else {
            std::cerr << "Errore durante l'inizio della transazione: " << PQresultErrorMessage(res) << std::endl;
            PQclear(res);
        }

        //Chiude la connessione 
        PQfinish(conn);
    } else {
        std:: cerr << "Errore dutante l'apertura della connessione al database PostgreSQL." << std::endl;
    }
}