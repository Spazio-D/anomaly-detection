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

// Valore configurabile per rilevazione anomalie
#define theta 0.5

// Struttura dati per i valori dei sensori con valore e presenza medie
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

// Legge i dati da un database SQL e li salva nelle strutture dati
bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, PGconn *conn);

// Calcola la presenza delle anomalie nelle medie
void detectAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages);

// Salva i dati calcolati nel database SQL
bool saveAnomalySQL(std::map<std::string, std::vector<Average>> &averages, std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn);


#endif