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

#define W 100

// Struct for data read, in the format read time, sensor ID, read value.
struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};


bool readRedisData(redisContext *context, std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWithNull,  std::map<std::string, std::vector<Data>> &dataNoNull);

std::map<std::string, std::vector<Data>> createDataWindow(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataNoNull, int wStart, int wEnd);

std::map<std::string, double> averageValue(std::vector<std::string> &sensors, std::map<std::string, std::vector<Data>> &dataWindowNoNull);



#endif // MAIN_H