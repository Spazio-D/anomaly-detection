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

    std::sort(sensors.begin(), sensors.end(), sensorSorting);

    // Read data from Redis streams
    std::map<std::string, std::vector<Data>> dataVector;
    if(!readDataRedis(context, sensors, dataVector)){
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
        PQfinish(conn);
        return 1;
    }

    for(size_t i = 0; i<dataVector[sensors[0]].size() - W+1 ; i++){
        //std::cout << "CREANDO LE FINESTRE" << std::endl;
        dataWindow = createDataWindow(dataVector, i, W + i-1);
        //std::cout << "CALCOLANDO LE MEDIE" << std::endl;
        averages = averageValue(dataWindow);
        //std::cout << "CALCOLANDO LE COVARIANZE" << std::endl;
        covariances = covarianceValue(sensors, dataWindow, averages);
        //std::cout << "SALVANDO LE MEDIE" << std::endl;
        if(!saveAverageInPostgreSQL(averages, i, conn)){
            PQfinish(conn);
            return 1;
        }
        //std::cout << "SALVANDO LE COVARIANZE" << std::endl;
        // CI METTE TANTISSIMO A SALVARE LA COVARIANZE
        if(!saveCovarianceInPostgreSQL(covariances, i, conn)){
            PQfinish(conn);
            return 1;
        }

    }

    PQfinish(conn);
    redisFree(context);
    return 0;
}