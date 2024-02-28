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
#include </opt/homebrew/opt/hiredis/include/hiredis/hiredis.h>
#include <map>
#include </opt/homebrew/opt/libpq/include/libpq-fe.h>
#include <iomanip>
#include <tuple>



struct Dato {
    std::string sampleTime;
    std::string SensorID;
    std::string value;
};

struct DatoPerFinestre {
    int sampleTime;
    std::string SensorID;
    double value;
};

//Constanti
#define W 100

void salvaCovarianzeInPostgreSQL(const std::map<std::string, std::map<std::string, double>>& matriceCovarianza);

//std::map<std::string, std::map<std::string, double>> calcolaMatriceCovarianzaMedie(const std::map<std::string, double>& medie, const std::map<std::string, FinestraTemporale>& fineste);

std::map<std::tuple<std::string, std::string, int, int>, double> calcolaMatriceCovarianza(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, std::vector<std::string> sensori);

double calcolaCovarianzaTraSensori (const std::vector<double>& sensorX, const std::vector<double>& sensorY);

std::map<int, std::map<std::string, double>> mergeSensorData(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre);

std::map<std::string, std::vector<DatoPerFinestre>> creaFinestre(const std::map<std::string, std::vector<Dato>>& datiPerSensore);

void salvaMedieInPostgreSQL(const std::map<std::string, std::vector<std::tuple<int, int, double>>>& mediePerSensore);

std::map<std::string, std::vector<std::tuple<int, int, double>>> calcolaMedie(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre);

void inserisciDatiInPostgreSQL(const std::map<std::string, std::vector<Dato>>& datiRaggrupati);

std::map<std::string, std::vector<Dato>> leggiDatiRedis(redisContext *context, const std::vector<std::string>& sensori);

#endif // MAIN_H