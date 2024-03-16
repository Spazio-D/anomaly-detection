#include "main.h"

int main() {

    // Lettura file CSV e salvataggio dati
    std::vector<Data> dataVector;
    if(!readFileCSV(FILE_NAME_CSV, dataVector)){
        return 1;
    }

    // Connessione a Redis
    redisContext *context = redisConnect("127.0.0.1", 6379);
    if (context == NULL || context->err) {

        if (context) {
            std::cerr << "Errore nella connessione a Redis: " << context->errstr << std::endl;
            redisFree(context);
        } else {
            std::cerr << "Impossibile allocare il contesto di connessione a Redis." << std::endl;
        }

        return 1;
    }

    // Invio stream di dati a Redis
    for (Data data : dataVector) {

        std::string command = "XADD " + data.sensorID + " * sampleTime " + data.sampleTime + " value " + data.value;
        redisReply *reply = (redisReply *)redisCommand(context, command.c_str());
        if (reply == NULL) {
            std::cerr << "Errore nell'invio del command a Redis." << std::endl;
            return 1;
        }
    
        freeReplyObject(reply);
    }

    redisFree(context);
    return 0;
}