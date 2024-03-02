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



struct Data {
    std::string sampleTime;
    std::string sensorID;
    std::string value;
};

struct DatoPerFinestre {
    int sampleTime;
    std::string sensorID;
    double value;
};

//Constanti
#define W 100

void salvaCovarianzeInPostgreSQL(const std::map<std::string, std::map<std::string, double>>& matriceCovarianza);

//std::map<std::string, std::map<std::string, double>> calcolaMatriceCovarianzaMedie(const std::map<std::string, double>& medie, const std::map<std::string, FinestraTemporale>& fineste);

std::map<std::tuple<std::string, std::string, int, int>, double> calcolaMatriceCovarianza(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, std::vector<std::string> sensors);

double calcolaCovarianzaTrasensors (const std::vector<double>& sensorX, const std::vector<double>& sensorY);

std::map<int, std::map<std::string, double>> mergeSensorData(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre);

std::map<std::string, std::vector<DatoPerFinestre>> creaFinestre(const std::map<std::string, std::vector<Data>>& datiPersensor);

void salvaMedieInPostgreSQL(const std::map<std::string, std::vector<std::tuple<int, int, double>>>& mediePersensor);

std::map<std::string, std::vector<std::tuple<int, int, double>>> calcolaMedie(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre);

void inserisciDatiInPostgreSQL(const std::map<std::string, std::vector<Data>>& datiRaggrupati);

std::map<std::string, std::vector<Data>> readRedisData(redisContext *context, const std::vector<std::string>& sensors);

#endif // MAIN_H