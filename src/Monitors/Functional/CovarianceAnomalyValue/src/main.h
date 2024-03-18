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
#include <map>
#include <libpq-fe.h>
#include <iomanip>
#include <tuple>
#include <limits>
#include <cmath>

// Struttura per i dati dei sensori con presenza e valore anomalia delle medie
struct Data {
    std::string sensorID;
    std::string value;
    std::string sampleTime;
    double averageAnomalyValue;
    bool isAverageAnomaly;
};

// Struttura per i dati delle medie
struct Average {
    std::string sensorID;
    double value;
    int firstSampleTime;
    int lastSampleTime;
};

// Struttura per i dati delle covarianze
struct Covariance {
    std::string sensorID1;
    std::string sensorID2;
    double value;
    int firstSampleTime;
    int lastSampleTime;
};

// Struttura per i dati delle anomalie delle covarianze
struct AnomalyCovariance {
    std::string sensorID1;
    std::string sensorID2;
    double value;
    bool isAnomaly;
    int sampleTime;
};

// Legge i dati da SQL e li salva in una struttura dati
bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, std::vector<std::vector<std::vector<Covariance>>> &covariances, PGconn *conn);

// Ordina i sensori in modo crescente per le cifre dell'id
bool sensorSorting(const std::string &str1, const std::string &str2);

// Calcola il valore dell'anomalia di una matrice di covarianze
std::vector<std::vector<std::vector<AnomalyCovariance>>> calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, std::vector<std::vector<std::vector<Covariance>>> &covariances);

// Salva i dati su SQL
bool saveAnomalySQL(std::vector<std::vector<std::vector<AnomalyCovariance>>> &covarianceAnomalyVector, PGconn *conn);

#endif // MAIN_H