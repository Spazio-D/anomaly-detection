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

struct AnomalyCovariance {
    std::string sensorID1;
    std::string sensorID2;
    double value;
    bool isAnomaly;
    int sampleTime;
};

#define theta 0.5

bool detectAnomaly(double anomalyValue);

bool updateDataSQL(PGconn *conn);


#endif // MAIN_H