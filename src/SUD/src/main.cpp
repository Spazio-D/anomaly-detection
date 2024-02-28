#include "main.h"

int main() {
    
    // Connection to Redis
    redisContext *context = redisConnect("127.0.0.1", 6379);

    if (context == NULL || context->err) {
        if (context) {
            std::cerr << "Errore nella connessione a Redis: " << context->errstr << std::endl;
            redisFree(context);
        } else {
            std::cerr << "Impossibile allocare il contesto di connessione a Redis." <<std::endl;
        }
        return 1;
    }
    
    //Stampa indirizzo IP e porta - serve per debug
    //std::cout << "Connessione a Redis riuscita. Indirizzo IP: " << context->tcp.host << ", Porta: " << context->tcp.port << std::endl;

    //Leggi dinamicamente l'elenco dei sensori disponibili da Redis
    redisReply *reply_sensori = (redisReply *)redisCommand(context, "KEYS *");

    if (reply_sensori == NULL || reply_sensori->type == REDIS_REPLY_ERROR) {
        std::cerr << "Errore nel recupero dell'elenco dei sensori da Redis." << std::endl;
        
        //debug
        // if (reply_sensori != NULL && reply_sensori->str != NULL) {
        //     std::cerr << " Messaggio di errore: " << reply_sensori->str;
        // }

        freeReplyObject(reply_sensori);
        redisFree(context);
        return 1;
    }

    // //Stampa l'elenco dei sensori - serve per debug
    // std::cout << "Elenco dei sensori:";
    // for (size_t i = 0; i < reply_sensori->elements; i +=1) {
    //     std::cout << " " << reply_sensori->element[i]->str << "\n";
    // }
    // std::cout << std::endl;

    //Estrai i nomi dei sensori dall'output di Redis
    std::vector<std::string> sensori;
    for (size_t i = 0; i < reply_sensori->elements; i +=1) {
        sensori.push_back(reply_sensori->element[i]->str);
    }

    freeReplyObject(reply_sensori);

    //Leggi i dati da Redis
    std::map<std::string, std::vector<Dato>> datiRaggrupati = leggiDatiRedis(context, sensori);

    //Inserisce dati in database 
    //inserisciDatiInPostgreSQL(datiRaggrupati);

    //Crea finestre temporali 
    std::map<std::string, std::vector<DatoPerFinestre>> finestre = creaFinestre(datiRaggrupati);

    //Calcola le medie per ogni sensore
    std::map<std::string, std::vector<std::tuple<int, int, double>>> mediePerSesnore = calcolaMedie(finestre);

    //Chiama la funzione per salvare le medie nel database
    //salvaMedieInPostgreSQL(mediePerSesnore);

    //Chaiama la funzione per calcolare la matrice di covariaznza tra i sensori
    std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza =  calcolaMatriceCovarianza(finestre, sensori);

    //Chiama la funzione per salvare le covarianze nel database
    //salvaCovarianzeInPostgreSQL(matriceCovarianza);

    // //Deb stampa la matrice di covarianza
    // for (const auto& entry : matriceCovarianza) {
    //     const auto& tupla = entry.first;
    //     const auto& valore = entry.second;

    //     //Estrae i valori dalla tupla
    //     std::string sensore1 = std::get<0>(tupla);
    //     std::string sensore2 = std::get<1>(tupla);
    //     int startSampleTime = std::get<2>(tupla);
    //     int endSampleTime = std::get<3>(tupla);

    //     std::cout << "Sensore 1: " << sensore1 << ", Sensore 2: " << sensore2 << ", Start Sample Time: " << startSampleTime << ", End Sample Time: " << endSampleTime << ", Covarianza: " << valore << std::endl;
    // }


    // //Deb funzione per stampare i valori della mappa mediPerSensore
    // for (const auto& coppia : mediePerSesnore) {
    //     const std::string& sensoreID = coppia.first;
    //     const auto& mediePerFinestra = coppia.second;

    //     std::cout << "Sensore ID: " << sensoreID << std::endl;
    //     for (const auto& tupla : mediePerFinestra) {
    //         int stratSampleTime = std::get<0>(tupla);
    //         int endSampleTime = std::get<1>(tupla);
    //         double media = std::get<2>(tupla);

    //         std::cout << "Start Sample Time: " << stratSampleTime << ", End Sample Time: " << endSampleTime << ", Media: " << media << std::endl;
    //     }
    //     std::cout << std::endl;
    // }

    // //Calcola le medie e le mette in map medieDaSalvare
    // std::map<std::string,double> medieDaSalvare;
    // //std::cout << "Media per ogni sensore: " << std::endl;
    // for (const auto& coppia : finestre) {
    //     const std::string& sensoreID = coppia.first;
    //     const FinestraTemporale& finestra = coppia.second;

    //     //std::cout << "Sensore: " << sensoreID << std::endl;
    //     //Calcola la media
    //     double media = calcolaMedia(finestra);

    //     //Stampa il risultato
    //     //std::cout << "SensorID: " << sensoreID << ", Media: " << media << std::endl;

    //     //Aggiungi la media alla mappa
    //     medieDaSalvare[sensoreID] = media;
    // }

    // //Deb stampa la matrice
    // std::cout << "Matrice di covarianza tra medie:" << std::endl;
    // for (const auto& coppia1 : matriceCovarianza) {
    //     const std::string& stream1 = coppia1.first;

    //     for (const auto& coppia2 : coppia1.second) {
    //         const std::string& stream2 = coppia2.first;
    //         const double covarianza = coppia2.second;

    //         // Stampa il valore nella matrice
    //         std::cout << "Covarianza(" << stream1 << ", " << stream2 << ") = " << covarianza << std::endl;
    //     }
    // }

    // //Deb stampa finestre
    // std::cout << "Finestre temporali: " << std::endl;
    // for (const auto& coppia : finestre) {
    //     const std::string& sensorID = coppia.first;
    //     const FinestraTemporale& finestra = coppia.second;

    //     std::cout << "SensorID: " << sensorID << std::endl;
    //     std::cout << "SampleTime: ";
    //     for (const std::string& sampleTime : finestra.sampleTime) {
    //         std::cout << sampleTime << " ";
    //     }
    //     std::cout << std::endl;

    //     std::cout << "Values: ";
    //     for (double value : finestra.values) {
    //         std::cout << value << " ";
    //     }
    //     std::cout << std::endl;
    //     std::cout << std::endl;
    // }

    // // Output di debug per vedere i dati letti da Redis
    // std::cout << "Dati letti da Redis:" << std::endl;
    // for (const auto& coppia : datiRaggrupati) {
    //     for (const auto& dato : coppia.second) {
    //         std::cout << "SensorID: " << dato.SensorID << ", SampleTime: " << dato.sampleTime << ", Value: " << dato.value << std::endl;
    //     }
    // }

    //Chiusura della conessione a Redis
    redisFree(context);

    return 0;
}