#include "main.h"

int main() {
    
    // Connection to Redis
    redisContext *context = redisConnect("127.0.0.1", 6379);
    if (context == NULL || context->err) {
        if (context) {
            std::cerr << "Errore nella connessione a Redis: " << context->errstr << std::endl;
            redisFree(context);
        } else {
            std::cerr << "Impossibile allocare il contesto di connessione da Redis." <<std::endl;
        }
        return 1;
    }
    
    // Collecting Redis streams keys
    redisReply *reply_sensors = (redisReply *)redisCommand(context, "KEYS *");
    if (reply_sensors == NULL || reply_sensors->type == REDIS_REPLY_ERROR) {
        std::cerr << "Errore nel recupero dell'elenco dei sensori da Redis." << std::endl;
        freeReplyObject(reply_sensors);
        redisFree(context);
        return 1;
    }

    std::vector<std::string> sensors;
    for (size_t i = 0; i < reply_sensors->elements; i ++) {
        sensors.push_back(reply_sensors->element[i]->str);
    }

    freeReplyObject(reply_sensors);

    std::sort(sensors.begin(), sensors.end());

    // Read data from Redis streams
    std::map<std::string, std::vector<Data>> dataVector;
    if(!readRedisData(context, sensors, dataVector)){
        return 1;
    }

    std::map<std::string, std::vector<Data>> dataWindow;
    std::map<std::string, double> averages;
    std::vector<std::vector<double>> covariances;

    PGconn *conn = PQconnectdb("dbname=anomalydetection user=ned password=47002 hostaddr=127.0.0.1 port=5432");
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Errore nella connessione a PostgreSQL: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    if (!saveDataInPostgreSQL(dataVector, conn)){
        return 1;
    }

    for(size_t i = 0; i<dataVector["SAC0"].size() - W + 1 ; i++){
        
        dataWindow = createDataWindow(dataVector, i, W + i - 1);
        averages = averageValue(dataWindow);
        covariances = covarianceValue(sensors, dataWindow, averages);

        if(!saveAverageInPostgreSQL(averages, i, conn)){
            return false;
        }
        
        if(!saveCovarianceInPostgreSQL(covariances, i, conn)){
            return false;
        }

    }




   
    

    //Chiama la funzione per salvare le medie nel database
    //salvaMedieInPostgreSQL(mediePerSesnore);

    //Chaiama la funzione per calcolare la matrice di covariaznza tra i sensors
    ////////std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza =  calcolaMatriceCovarianza(finestre, sensors);


    //Chiusura della conessione a Redis
    redisFree(context);

    return 0;
}