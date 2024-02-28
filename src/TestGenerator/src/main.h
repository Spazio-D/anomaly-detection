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


// Struttura per i dati letti dai sensori del tipo: Tempo di lettura, ID del sensore, Valore letto
struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};

std::vector<Data> readFileCSV(const std::string& fileName);

#define fileNameCSV "../../../Data/data1273117654.csv"

#endif 