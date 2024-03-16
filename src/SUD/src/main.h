#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>
#include <set>
#include <string>
#include <hiredis/hiredis.h>
#include <map>
#include <libpq-fe.h>
#include <iomanip>
#include <tuple>
#include <limits>
#include <cmath>

// Grandezza della finestra temporale
#define WINDOW_SIZE 5


// Struttura per i dati letti da Redis
struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};


// Legge le stream di dati da Redis e salva il contenuto in un vettore di dati
bool readDataRedis(redisContext *context, std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataVector);

// Crea una finestra temporale di dati
std::map<std::string, std::vector<Data>> createDataWindow(std::map<std::string, std::vector<Data>> &dataVector, int wStart, int wEnd);

// Calcola le medie dei dati
std::map<std::string, double> averageValue(std::map<std::string, std::vector<Data>> &dataWindow);

// Calcola le covarianze dei dati
std::vector<std::vector<double>> covarianceValue(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWindow, std::map<std::string, double> &averages);

// Salva i dati in PostgreSQL
bool saveDataInPostgreSQL(std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn);

// Salva le medie in PostgreSQL
bool saveAverageInPostgreSQL(std::map<std::string, double> &averages, size_t firstSampleTime, PGconn *conn);

// Salva le covarianze in PostgreSQL
bool saveCovarianceInPostgreSQL(std::vector<std::vector<double>> &covariances, size_t firstSampleTime, PGconn *conn);

// Confronta i nomi dei sensori
bool sensorSorting(const std::string &str1, const std::string &str2);


#endif