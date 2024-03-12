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

// Struct for data read, in the format read time, sensor ID, read value.
struct Data {
    std::string sensorID;
    std::string value;
    std::string sampleTime;
    double averageAnomalyValue;
    bool isAverageAnomaly;
};

struct Average {
    std::string sensorID;
    double value;
    int firstSampleTime;
    int lastSampleTime;
};

struct Covariance {
    std::string sensorID1;
    std::string sensorID2;
    double value;
    int firstSampleTime;
    int lastSampleTime;
};

struct AnomalyCovariance {
    std::string sensorID1;
    std::string sensorID2;
    double value;
    bool isAnomaly;
    int sampleTime;
};

bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, std::vector<std::vector<std::vector<Covariance>>> &covariances, PGconn *conn);

bool sensorSorting(const std::string &str1, const std::string &str2);

std::vector<std::vector<std::vector<AnomalyCovariance>>> calculateAnomaly(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, std::vector<Average>> &averages, std::vector<std::vector<std::vector<Covariance>>> &covariances);

bool saveAnomalySQL(std::vector<std::vector<std::vector<AnomalyCovariance>>> &covarianceAnomalyVector, PGconn *conn);

#endif // MAIN_H