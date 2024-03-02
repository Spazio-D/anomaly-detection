// #include "main.h"


// std::map<std::tuple<std::string, std::string, int, int>, double> calcolaMatriceCovarianza(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, std::vector<std::string> sensors) {
//     std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza;

//     // Unisce i dati dei sensors
//     auto mergedData = mergeSensorData(finestre);

//     // Calcola la covarianza per ogni coppia di sensors
   
//     for (size_t i = 0; i < sensors.size(); ++i) {
//         const std::string& sensor1 = sensors[i];

//         // Trova i tempi di campionamento comuni tra i due sensors
//         std::set<int> commonSampleTimes;
//         for (const auto& data : mergedData) {
//             if (data.second.count(sensor1)) {
//                 for (size_t j = i + 1; j < sensors.size(); ++j) {
//                     const std::string& sensor2 = sensors[j];
//                     if (data.second.count(sensor2)) {
//                         commonSampleTimes.insert(data.first);
//                         break;
//                     }
//                 }
//             }
//         }

//         // Calcola la covarianza per ogni tempo di campionamento comune
//         for (const int sampleTime : commonSampleTimes) {
//             int endSampleTime = sampleTime + W - 1;

//             // Calcola la covarianza solo se la finestra temporale corrente non va oltre la fine dei dati disponibili
//             if (mergedData.count(endSampleTime)) {
//                 std::map<std::string, std::vector<double>> sensorValues;

//                 // Raccogli i valori dei sensors nella finestra temporale corrente
//                 for (const auto& sensor : sensors) {
//                     if (mergedData[endSampleTime].count(sensor)) {
//                         sensorValues[sensor] = std::vector<double>(W);
//                         std::transform(
//                             // modificato 27 02 12:57 elisa 
//                             // --mergedData[endSampleTime][sensor].begin(),
//                             // --mergedData[endSampleTime][sensor].end(),
//                             mergedData[endSampleTime].begin(),
//                             mergedData[endSampleTime].end(),
//                             sensorValues[sensor].begin(),
//                             [](const auto& pair) { return pair.second; }
//                         );
//                     }
//                 }

//                 // Calcola la covarianza tra le coppie di sensors
//                 for (size_t j = i + 1; j < sensors.size(); ++j) {
//                     const std::string& sensor2 = sensors[j];
//                     if (sensorValues.count(sensor2)) {
//                         double covarianza = calcolaCovarianzaTrasensors(sensorValues[sensor1], sensorValues[sensor2]);
                        
//                         {
//                             matriceCovarianza[{sensor1, sensor2, sampleTime, endSampleTime}] = covarianza;
//                             std::cout << "sensor1: " << sensor1 << ", sensor2: " << sensor2 << ", StartSampleTime: " << sampleTime << ", EndSampleTime: " << endSampleTime << " - Covarianza: " << covarianza << std::endl;
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     return matriceCovarianza;
// }
