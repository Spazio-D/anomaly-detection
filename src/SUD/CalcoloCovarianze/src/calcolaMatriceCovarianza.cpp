#include "main.h"

// //Funzione per calcolare la matrice di covarianza tra le medie
// std::map<std::string, std::map<std::string, double>> calcolaMatriceCovarianzaMedie(const std::map<std::string, double>& medie, const std::map<std::string, FinestraTemporale>& fineste) {
//     std::map<std::string, std::map<std::string, double>> matriceCovarianza;
//     //Set per tenere traccia delle coppie elaborate
//     std::set<std::pair<std::string, std::string>> coppieElaborate;

//     //Intera su ogni coppia di steams
//     for (const auto& coppia1 : medie) {
//         const std::string& stream1 = coppia1.first;

//         for (const auto& coppia2 : medie) {
//             const std::string& stream2 = coppia2.first;

//             if (stream1 != stream2 && coppieElaborate.count({stream1, stream2}) == 0 && coppieElaborate.count({stream2, stream1}) == 0) {
//                 //Calcola la covarianza tra i due sensori
//                 double covarianza = calcolaCovarianzaTraSensori(fineste.at(stream1).values, fineste.at(stream2).values);

//                 //Salva il risultato nella matrice
//                 matriceCovarianza[stream1][stream2] = covarianza;

//                 //Aggiunge la coppia al set delle coppie elaborate 
//                 coppieElaborate.insert({stream1, stream2});
//             }
//         }
//     }

//     return matriceCovarianza;
// }

// //Funzione per calcolare la matrice di covarianza
// std::map<std::tuple<std::string, std::string, int, int>, double> calcolaMatriceCovarianza(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, std::vector<std::string> sensori) {
//     std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza;

//     //Calcola la covarianza per ogni coppia di sensori
//     for (size_t i = 0; i < sensori.size(); ++i) {
//         for (size_t j = i + 1; j < sensori.size(); ++j) {
//             const std::string& sensore1 = sensori[i];
//             const std::string& sensore2 = sensori[j];

//             //Itera attraverso le finestre temporali 
//             for (size_t k = 0; k <= finestre.at(sensore1).size() - W; ++k) {
//                 //Seleziona i valori corrispondenti dei due sensori nella finestra temporale corrente
//                 std::vector<double> valuesSensor1;
//                 std::vector<double> valuesSensor2;
                
//                 //Indici per iterare attraverso i campioni temporali dei due sensori
//                 int idx1 = k, idx2 = k;
//                 while (idx1 < k + W && idx2 < k + W) {
//                     int sampleTime1 = finestre.at(sensore1)[idx1].sampleTime;
//                     int sampleTime2 = finestre.at(sensore2)[idx2].sampleTime;
//                     if (sampleTime1 == sampleTime2) {
//                         //Se i campioni temporali corrispondono, aggiunge i valori ai vettori
//                         valuesSensor1.push_back(finestre.at(sensore1)[idx1].value);
//                         valuesSensor2.push_back(finestre.at(sensore2)[idx2].value);
//                         ++idx1;
//                         ++idx2;
//                     } else if (sampleTime1 < sampleTime2) {
//                         //Se il campione temporale del sensore 1 è minore, incrementa l'indice per il sensore1
//                         ++idx1;
//                     } else {
//                         //Se il campione temporale del sensore 2 è minore, incrementa l'indice per il sensore2
//                         ++idx2;
//                     }
//                 }

//                 if (!valuesSensor1.empty() && !valuesSensor2.empty()) {
//                     //Calcola la covarianza tra i vettori dei due sensori nella finestra temporale corrente
//                     double covarianza = calcolaCovarianzaTraSensori(valuesSensor1, valuesSensor2);

//                     //Salva il risultato nella matrice di covarianza
//                     matriceCovarianza[{sensore1, sensore2, finestre.at(sensore1)[k].sampleTime, finestre.at(sensore1)[k + W - 1].sampleTime}] = covarianza;
                    
//                     std::cout << "Sensore1: " << sensore1 << ", Sensore2: " << sensore2 << ", StartSampleTinme: " << finestre.at(sensore1)[k].sampleTime << ", endSampleTime: " << finestre.at(sensore1)[k + W - 1].sampleTime << " - covarianza: " << covarianza << std::endl;
//                 }
//             }
//         }
//     }

//     return matriceCovarianza;
// }

// //Funzione per calcolare la matrice di covarianza
// std::map<std::tuple<std::string, std::string, int, int>, double> calcolaMatriceCovarianza(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, std::vector<std::string> sensori) {
//     std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza;

//     //Creazione degli indici per i sample time dei sensori
//     std::map<std::string, std::map<int, size_t>> indiciSensori;
//     for (const auto& sensore : sensori) {
//         const auto& datiSensori = finestre.at(sensore);
//         for (size_t idx = 0; idx < datiSensori.size(); ++idx) {
//             indiciSensori[sensore][datiSensori[idx].sampleTime] = idx;
//         }
//     }

//     //Calcola la covarianza per ogni coppia di sensori
//     #pragma omp parallel for
//     for (size_t i = 0; i < sensori.size(); ++i) {
//         const std::string& sensore1 = sensori[i];
//         const auto& datiSensori1 = finestre.at(sensore1);
//         for (size_t j = i + 1; j < sensori.size(); ++j) {
//             const std::string& sensore2 = sensori[j];
//             const auto& datiSensori2 = finestre.at(sensore2);
//             const auto& indici1 = indiciSensori[sensore1];
//             const auto& indici2 = indiciSensori[sensore2];

//             //Itera attraverso li sample rime comuni ai due sensori
//             for (const auto& coppia : indici1) {
//                 int sampleTime = coppia.first;
//                 size_t idx1 = coppia.second;
//                 auto it2 = indici2.find(sampleTime);
//                 if (it2 != indici2.end()) {
//                     size_t idx2 = it2->second;

//                     //Seleziona i valori corrispondenti dei due sensori nella finestra temporale corrente
//                     std::vector<double> valuesSensor1;
//                     std::vector<double> valuesSensor2;

//                     //Aggiunge valori ai vettori
//                     for (int k = 0; k < W; k++) {
//                         valuesSensor1.push_back(datiSensori1[idx1 + k].value);
//                         valuesSensor2.push_back(datiSensori2[idx2 + k].value);
//                     }

//                     //Calcola la covarianza tra i vettori dei due sensori nella finestra temporale corrente
//                     double covarianza = calcolaCovarianzaTraSensori(valuesSensor1, valuesSensor2);

//                     //Salva il risultato nella matrice di covarianza
//                     #pragma omp critical
//                     {
//                         matriceCovarianza[{sensore1, sensore2, sampleTime, sampleTime + W - 1}] = covarianza;
                    
//                         std::cout << "Sensore1: " << sensore1 << ", Sensore2: " << sensore2 << ", StartSampleTinme: " << sampleTime << ", endSampleTime: " << sampleTime + W - 1 << " - covarianza: " << covarianza << std::endl;
//                     }
//                 }
//             }
//         }
//     }

//     return matriceCovarianza;
// }

// //Funzione per calcolare la matrice di covarianza tra le medie
// std::map<std::tuple<std::string, std::string, int, int>, double> calcolaMatriceCovarianza(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, std::vector<std::string> sensori) {
//     std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza;

//     //Unisce i dati dei sensori
//     auto mergedData = mergeSensorData(finestre);

//     //Calcola la covarianza per ogni coppia di sensori
//     for (size_t i = 0; i < sensori.size(); ++i) {
//         const std::string& sensore1 = sensori[i];
//         for (size_t j = i + 1; j < sensori.size(); ++j) {
//             const std::string& sensore2 = sensori[j];

//             //Trova i tempi di campionamento comuni tra i due sensori
//             std::set<int> commonSampleTimes;
//             for (const auto& data : mergedData) {
//                 if (data.second.count(sensore1) && data.second.count(sensore2)) {
//                     commonSampleTimes.insert(data.first);
//                 }
//             }

//             //Calcola la covarianza per ogni tempo di campionamento comune
//             for (const int sampleTime : commonSampleTimes) {
//                 //Calcola l'indice finale della finestra temporale corrente
//                 int endSampleTime = sampleTime + W - 1;

//                 //Calcola la covarianza solo se la finestra temporale corrente non va oltre la fine dei dati disponibili
//                 if (mergedData.count(endSampleTime) && mergedData[endSampleTime].count(sensore1) && mergedData[endSampleTime].count(sensore2)) {
//                     //Calcola la covarianza tra i vettori dei due sensori nella finestra temporale corrente
//                     double covarianza = calcolaCovarianzaTraSensori(mergedData, sensore1, sensore2, sampleTime);

//                     //Salva il risultato nella matrice di covarianza
//                     #pragma omp critical
//                     {
//                         matriceCovarianza[{sensore1, sensore2, sampleTime, endSampleTime}] = covarianza;
                    
//                         std::cout << "Sensore1: " << sensore1 << ", Sensore2: " << sensore2 << ", StartSampleTinme: " << sampleTime << ", endSampleTime: " << endSampleTime << " - covarianza: " << covarianza << std::endl;
//                     }
//                 }
//             }
//         }
//     }

//     return matriceCovarianza;
// }


std::map<std::tuple<std::string, std::string, int, int>, double> calcolaMatriceCovarianza(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, std::vector<std::string> sensori) {
    std::map<std::tuple<std::string, std::string, int, int>, double> matriceCovarianza;

    // Unisce i dati dei sensori
    auto mergedData = mergeSensorData(finestre);

    // Calcola la covarianza per ogni coppia di sensori
   
    for (size_t i = 0; i < sensori.size(); ++i) {
        const std::string& sensore1 = sensori[i];

        // Trova i tempi di campionamento comuni tra i due sensori
        std::set<int> commonSampleTimes;
        for (const auto& data : mergedData) {
            if (data.second.count(sensore1)) {
                for (size_t j = i + 1; j < sensori.size(); ++j) {
                    const std::string& sensore2 = sensori[j];
                    if (data.second.count(sensore2)) {
                        commonSampleTimes.insert(data.first);
                        break;
                    }
                }
            }
        }

        // Calcola la covarianza per ogni tempo di campionamento comune
        for (const int sampleTime : commonSampleTimes) {
            int endSampleTime = sampleTime + W - 1;

            // Calcola la covarianza solo se la finestra temporale corrente non va oltre la fine dei dati disponibili
            if (mergedData.count(endSampleTime)) {
                std::map<std::string, std::vector<double>> sensorValues;

                // Raccogli i valori dei sensori nella finestra temporale corrente
                for (const auto& sensore : sensori) {
                    if (mergedData[endSampleTime].count(sensore)) {
                        sensorValues[sensore] = std::vector<double>(W);
                        std::transform(
                            // modificato 27 02 12:57 elisa 
                            // --mergedData[endSampleTime][sensore].begin(),
                            // --mergedData[endSampleTime][sensore].end(),
                            mergedData[endSampleTime].begin(),
                            mergedData[endSampleTime].end(),
                            sensorValues[sensore].begin(),
                            [](const auto& pair) { return pair.second; }
                        );
                    }
                }

                // Calcola la covarianza tra le coppie di sensori
                for (size_t j = i + 1; j < sensori.size(); ++j) {
                    const std::string& sensore2 = sensori[j];
                    if (sensorValues.count(sensore2)) {
                        double covarianza = calcolaCovarianzaTraSensori(sensorValues[sensore1], sensorValues[sensore2]);
                        
                        {
                            matriceCovarianza[{sensore1, sensore2, sampleTime, endSampleTime}] = covarianza;
                            std::cout << "Sensore1: " << sensore1 << ", Sensore2: " << sensore2 << ", StartSampleTime: " << sampleTime << ", EndSampleTime: " << endSampleTime << " - Covarianza: " << covarianza << std::endl;
                        }
                    }
                }
            }
        }
    }

    return matriceCovarianza;
}
