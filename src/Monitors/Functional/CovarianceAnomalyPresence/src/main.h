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

// Struttura per i dati delle anomalie delle covarianze
struct AnomalyCovariance {
    std::string sensorID1;
    std::string sensorID2;
    double value;
    bool isAnomaly;
    int sampleTime;
};

// Valore limite configurabile
#define theta 0.5

// Legge un valore e calcola se è anomalo
bool detectAnomaly(double anomalyValue);

// Aggiorna i dati sul database SQL
bool updateDataSQL(PGconn *conn);


#endif // MAIN_H