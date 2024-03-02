#include "main.h"

// Legge i dati dalle stream Redis e li mappa in due variabili, una completa e una contentente solo i valori diversi da NULL
bool readRedisData(redisContext *context, std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWithNull,  std::map<std::string, std::vector<Data>> &dataNoNull) {

    for (std::string &sensor : sensors) {

        std::string command = "XRANGE " + sensor + " - +";
        redisReply *reply = (redisReply *)redisCommand(context, command.c_str());

        if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
             std::cerr << "Errore nella lettura della stream da Redis per il sensore " << sensor << std::endl;
            return false;
        }

        Data newData;
        for (size_t i = 0; i < reply->elements; i++){

            newData.sampleTime = reply->element[i]->element[1]->element[1]->str;
            newData.value = reply->element[i]->element[1]->element[3]->str;
            newData.sensorID = sensor;
            dataWithNull[sensor].push_back(newData);

            std::cout << dataWithNull[sensor][i].sensorID << "\t" << dataWithNull[sensor][i].sampleTime << "\t" << dataWithNull[sensor][i].value << std::endl;


            if(newData.value != "NULL"){
                dataNoNull[sensor].push_back(newData);
                //std::cout << dataNoNull[sensor][i].sensorID << "\t " << dataWithNull[sensor][i].sampleTime << "\t" << dataWithNull[sensor][i].value << std::endl;
            }
        }

        freeReplyObject(reply);
    }

    return true;
}