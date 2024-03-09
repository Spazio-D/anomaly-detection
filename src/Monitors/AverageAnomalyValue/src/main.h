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

#define theta 0.5

// Struct for data read, in the format read time, sensor ID, read value.
struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};

bool readDataSQL(std::map<std::string, std::vector<Data>> &dataVector, std::map<std::string, double> averages, PGconn *conn);

#endif // MAIN_H