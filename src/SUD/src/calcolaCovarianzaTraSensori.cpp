// #include "main.h"

// //Funzione per calcolare la covaarianza tra due sensors
// double calcolaCovarianzaTrasensors (const std::vector<double>& sensorX, const std::vector<double>& sensorY) {
//     //Assicurati che entrambi i vettori abbiano almeno un valore
//     if (sensorX.empty() || sensorY.empty()){
//         std::cerr << "Errore: i vettori devono contenere almeno un elemento per calcolare la covarianza." << std::endl;
//         return 0.0; //o un altro valore di default
//     }

//     size_t n = sensorX.size();

//     //Calcola le medie dei due sensors
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
//     covarianza /= n; //divisione per n

//     return covarianza;
// }

