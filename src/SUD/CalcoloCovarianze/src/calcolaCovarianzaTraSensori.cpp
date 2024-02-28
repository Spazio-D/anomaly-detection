#include "main.h"

// //Funzione per calcolare la covaarianza tra due sensori
// double calcolaCovarianzaTraSensori (const std::vector<double>& sensorX, const std::vector<double>& sensorY) {
//     //Assicurati che entrambi i vettori abbiano almeno un valore
//     if (sensorX.empty() || sensorY.empty()){
//         std::cerr << "Errore: i vettori devono contenere almeno un elemento per calcolare la covarianza." << std::endl;
//         return 0.0; //o un altro valore di default
//     }

//     //Assicura che entrambi i vettori abbiano la stessa imensione
//     size_t n = std::min(sensorX.size(), sensorY.size());

//     //Calcola le medie dei due sensori
//     double mediaX = 0.0, mediaY = 0.0;
//     for(size_t i = 0; i < n ; ++i){
//         mediaX += sensorX[i];
//         mediaY += sensorY[i];
//     }

//     mediaX /= n;
//     mediaY /= n;

//     // Calcola la covarianza
//     double covarianza = 0.0;
//     for (size_t i = 0; i < n; ++i) {
//         covarianza += (sensorX[i] - mediaX) * (sensorY[i] - mediaY);
//     }
//     covarianza /= (n - 1); //divisione per n-1

//     return covarianza;
// }

//Funzione per calcolare la covaarianza tra due sensori
double calcolaCovarianzaTraSensori (const std::vector<double>& sensorX, const std::vector<double>& sensorY) {
    //Assicurati che entrambi i vettori abbiano almeno un valore
    if (sensorX.empty() || sensorY.empty()){
        std::cerr << "Errore: i vettori devono contenere almeno un elemento per calcolare la covarianza." << std::endl;
        return 0.0; //o un altro valore di default
    }

    size_t n = sensorX.size();

    //Calcola le medie dei due sensori
    double mediaX = 0.0, mediaY = 0.0;
    for(size_t i = 0; i < n ; ++i){
        mediaX += sensorX[i];
        mediaY += sensorY[i];
    }

    mediaX /= n;
    mediaY /= n;

    // Calcola la covarianza
    double covarianza = 0.0;
    for (size_t i = 0; i < n; ++i) {
        covarianza += (sensorX[i] - mediaX) * (sensorY[i] - mediaY);
    }
    covarianza /= n; //divisione per n

    return covarianza;
}

// // Funzione per calcolare la covarianza tra due sensori basata sui dati unificati
// double calcolaCovarianzaTraSensori(const std::map<int, std::map<std::string, double>>& mergedData, const std::string& sensore1, const std::string& sensore2, int sampleTime) {
//     double covarianza = 0.0;
//     double mediaX = 0.0, mediaY = 0.0;
//     size_t n = 0;

//     // Calcola le medie dei due sensori
//     for (const auto& data : mergedData) {
//         if (data.second.count(sensore1) && data.second.count(sensore2)) {
//             mediaX += data.second.at(sensore1);
//             mediaY += data.second.at(sensore2);
//             ++n;
//         }
//     }

//     if (n == 0) {
//         std::cerr << "Errore: Nessun dato disponibile per entrambi i sensori." << std::endl;
//         return covarianza; // o un altro valore di default
//     }

//     mediaX /= n;
//     mediaY /= n;

//     // Calcola la covarianza
//     for (const auto& data : mergedData) {
//         if (data.first == sampleTime && data.second.count(sensore1) && data.second.count(sensore2)) {
//             covarianza += (data.second.at(sensore1) - mediaX) * (data.second.at(sensore2) - mediaY);
//         }
//     }

//     covarianza /= n; // divisione per n
//     return covarianza;
// }

