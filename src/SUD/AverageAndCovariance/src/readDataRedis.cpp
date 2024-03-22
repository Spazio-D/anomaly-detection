#include "main.h"

bool readDataRedis(redisContext *context, std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataVector){

    // Scorrimento dei sensori per la lettura dei dati da Redis
    for (std::string sensor : sensors) {

        // Lettura della stream da Redis
        std::string command = "XRANGE " + sensor + " - +";
        redisReply *reply = (redisReply *)redisCommand(context, command.c_str());
        if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
            std::cerr << "Errore nella lettura della stream da Redis per il sensore " << sensor << std::endl;
            return false;
        }

        // Inserimento dei dati letti nella struttura dati
        for (size_t i = 0; i < reply->elements; i++){
            
            Data data;
            data.sampleTime = reply->element[i]->element[1]->element[1]->str;
            data.value = reply->element[i]->element[1]->element[3]->str;
            data.sensorID = sensor;
            dataVector[sensor].push_back(data);

        }

        freeReplyObject(reply);
    }

    return true;
}