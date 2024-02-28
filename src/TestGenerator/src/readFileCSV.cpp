#include "main.h"

std::vector<Data> readFileCSV(const std::string& fileName) {
    std::vector<Data> dataVector;
    std::ifstream fileCSV(fileName);

    if (!fileCSV.is_open()) {
        std::cerr << "Impossible to open the file: " << fileName << std::endl;
        return dataVector;
    }

    std::string line;
    std::getline(fileCSV, line);

    while (fileCSV.good()) {
        
        std::getline(fileCSV, line);

        Data data;
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());

        std::istringstream lineStream(line);
        lineStream >> data.sampleTime >> data.sensorID >> data.value;
        
        if (data.sensorID.length()<=4 && std::stoi(data.sampleTime)<10){
            // std::cout << data.sensorID << std::endl;
            dataVector.push_back(data);
        }
        
    }

    fileCSV.close();
    return dataVector;
}