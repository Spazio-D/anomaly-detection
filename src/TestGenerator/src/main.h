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

// Path del file CSV contenente i dati
#define FILE_NAME_CSV "../Data/data1273117654.csv"

// Numero di sensori da leggere
#define SENSOR 15

// Numero di sampletime da leggere
#define SAMPLETIME 50


// Struttura per i dati letti dal file CSV
struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};


// Legge un file CSV e salva il contenuto in un vettore di dati
bool readFileCSV(std::string fileName, std::vector<Data> &dataVector);

#endif 