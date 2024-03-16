#include "main.h"


bool readFileCSV(std::string fileName, std::vector<Data> &dataVector) {
    
    // Apertura file CSV
    std::ifstream fileCSV(fileName);
    if (!fileCSV.is_open()) {
        std::cerr << "Impossibile aprire il file CSV al path " << fileName << std::endl;
        return false;
    }

    // Lettura file CSV
    std::string line;
    std::getline(fileCSV, line);
    while (fileCSV.good()) {
        
        // Spacchettamento dati
        Data data;
        std::getline(fileCSV, line);
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());
        std::istringstream lineStream(line);
        lineStream >> data.sampleTime >> data.sensorID >> data.value;
        
        // Controllo dati da prendere
        if(std::stoi(data.sensorID.substr(3)) <= SENSOR && std::stoi(data.sampleTime) <= SAMPLETIME){
            dataVector.push_back(data);
        }

    }

    fileCSV.close();
    return true;
}