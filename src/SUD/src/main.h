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

#define W 4

// Struct for data read, in the format read time, sensor ID, read value.
struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};


bool readDataRedis(redisContext *context, std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataVector);

std::map<std::string, std::vector<Data>> createDataWindow(std::map<std::string, std::vector<Data>> &dataVector, int wStart, int wEnd);

std::map<std::string, double> averageValue(std::map<std::string, std::vector<Data>> &dataWindow);

std::vector<std::vector<double>> covarianceValue(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWindow, std::map<std::string, double> &averages);

bool saveDataInPostgreSQL(std::map<std::string, std::vector<Data>> &dataVector, PGconn *conn);

bool saveAverageInPostgreSQL(std::map<std::string, double> &averages, size_t firstSampleTime, PGconn *conn);

bool saveCovarianceInPostgreSQL(std::vector<std::vector<double>> &covariances, size_t firstSampleTime, PGconn *conn);

bool sensorSorting(const std::string &str1, const std::string &str2);


#endif // MAIN_H