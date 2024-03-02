// #include "main.h"

// std::map<int, std::map<std::string, double>> mergeSensorData(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre) {
//     std::map<int, std::map<std::string, double>> mergedData;

//     for (const auto& coppia : finestre) {
//         const std::string& sensorID = coppia.first;
//         const std::vector<DatoPerFinestre>& datiFinestre = coppia.second;

//         for (const auto& dato: datiFinestre) {
//             mergedData[dato.sampleTime][sensorID] = dato.value;
//         }
//     }
//     return mergedData;
// }