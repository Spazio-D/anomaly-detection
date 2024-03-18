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

// Valore configurabile per la rilevazione delle anomalie
#define theta 0.5

// Struttura per i dati dei sensori con presenza e valore anomalie delle medie
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

// Legge da un database SQL e popola le strutture dati
bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, PGconn *conn);

// Calcola il valore dell'anomalia delle medie
void calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages);

// Salva il valore delle anomalie nel database
bool saveAnomalySQL(std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn);


#endif