#include "main.h"

// Reads a CSV file and saves the processed content into a data vector
bool readFileCSV(std::string fileName, std::vector<Data> &dataVector) {
    
    std::ifstream fileCSV(fileName);

    if (!fileCSV.is_open()) {
        std::cerr << "Impossibile aprire il file CSV al path " << fileName << std::endl;
        return false;
    }

    std::string line;
    std::getline(fileCSV, line);
    Data data;

    while (fileCSV.good()) {
        
        std::getline(fileCSV, line);
        
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());

        std::istringstream lineStream(line);
        lineStream >> data.sampleTime >> data.sensorID >> data.value;
        
        if (data.sensorID.length()<=4 && std::stoi(data.sampleTime)<10){
            dataVector.push_back(data);
        }

    }

    fileCSV.close();
    return true;
}