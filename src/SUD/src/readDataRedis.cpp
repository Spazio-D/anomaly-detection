#include "main.h"

// Legge i dati dalle stream Redis e li mappa in due variabili, una completa e una contentente solo i valori diversi da NULL
bool readDataRedis(redisContext *context, std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataVector){

    Data data;

    for (std::string sensor : sensors) {

        //std::cout << sensor << std::endl;
        std::string command = "XRANGE " + sensor + " - +";
        redisReply *reply = (redisReply *)redisCommand(context, command.c_str());

        if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
            std::cerr << "Errore nella lettura della stream da Redis per il sensore " << sensor << std::endl;
            return false;
        }

        for (size_t i = 0; i < reply->elements; i++){

            data.sampleTime = reply->element[i]->element[1]->element[1]->str;
            data.value = reply->element[i]->element[1]->element[3]->str;
            data.sensorID = sensor;
            dataVector[sensor].push_back(data);

            //std::cout << dataVector[sensor][i].sensorID << "\t" << dataVector[sensor][i].sampleTime << "\t" << dataVector[sensor][i].value << std::endl;

        }

        freeReplyObject(reply);
    }

    return true;
}