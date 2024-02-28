#include "main.h"

int main() {

    std::vector<Data> dataVector = readFileCSV(fileNameCSV);

    // Connection to Redis
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

    // Send streams to Redis
    redisReply *reply;
    for (const Data data : dataVector) {

        std::string command = "XADD " + data.sensorID + " * sampleTime " + data.sampleTime + " value " + data.value;
        reply = (redisReply *)redisCommand(context, command.c_str());
        std::cout << data.sensorID << " " << data.sampleTime << " " << data.value <<std::endl;
        // std::cout << "a";
        if (reply == NULL) {
            std::cerr << "Errore nell'invio del comando a Redis." << std::endl;
            return 1;
        } else {
            freeReplyObject(reply);
        }
    }

    redisFree(context);
    return 0;
}