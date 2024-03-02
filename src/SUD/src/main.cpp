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


    // Read data from Redis streams
    std::map<std::string, std::vector<Data>> dataWithNull;
    std::map<std::string, std::vector<Data>> dataNoNull;
    if(!readRedisData(context, sensors, dataWithNull, dataNoNull)){
        return 1;
    }

    // i<dataWithNull["SAC0"].size()- W + 1 -- IL VALORE GIUSTO (SOSTITUIRE IL PARAMETRO DI createDataWindow con W + i al posto di 5 + i)
    for(size_t i = 0; i<6 ; i++){
        
        std::map<std::string, std::vector<Data>> dataWindowNoNull = createDataWindow(sensors, dataNoNull, i, 4 + i);
        std::map<std::string, double> averages = averageValue(sensors, dataWindowNoNull);

    }




   
    

    //Chiama la funzione per salvare le medie nel database
    //salvaMedieInPostgreSQL(mediePerSesnore);

    //Chaiama la funzione per calcolare la matrice di covariaznza tra i sensors
    ////////std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza =  calcolaMatriceCovarianza(finestre, sensors);


    //Chiusura della conessione a Redis
    redisFree(context);

    return 0;
}