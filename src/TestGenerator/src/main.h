#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <sstream>
#include <fstream>
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


#define FILE_NAME_CSV "../Data/data1273117654.csv"

// Struct for data read, in the format read time, sensor ID, read value.
struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};

bool readFileCSV(std::string fileName, std::vector<Data> &dataVector);

#endif 