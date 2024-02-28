#include "main.h"

// //Funzione per calcolare le media di una finestra temporale con lunghezza W
// double calcolaMedia(const FinestraTemporale& finestra, int W) {
//     if (finestra.values.empty() || W <= 0) {
//         return 0.0; //Restituisce 0 se non ci sono valori nella finestra o W è non positivo
//     }

//     double somma = 0.0;
//     int contatore = 0;
//     int numValori = finestra.values.size();

//     //Calcola la somma dei valori dalla finestra temporale iniziale a quella con indice W-1
//     for (int i = 0; i < W && i < numValori; ++i) {
//         somma += finesta.values[i];
//     }

//     double media = somma 
//     // std::cout << " media: " << somma / finestra.values.size() << " somma: " << somma << " lunghezza: " << finestra.values.size() << std::endl; 
//     return somma / finestra.values.size();

// }

// //Funzione per calcolare le medie su finestre temporali scorrevoloi di lunghezza W
// std::map<std::string, std::vector<std::tuple<int, int, double>>> calcolaMedie(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre, int W) {
//     std::map<std::string, std::vector<std::tuple<int, int, double>>> mediePerSensore;

//     //Itera su ogni finestra temporale
//     for (const auto& coppia : finestre) {
//         const std::string& sensoreID = coppia.first;
//         const std::vector<DatoPerFinestre>& datiFinestre = coppia.second;

//         //Vettore per salvare le medie per ogni finestra
//         std::vector<std::tuple<int, int, double>> mediePerFinestra;

//         //Inizializza il numero totale di SampleTime per il sensore
//         int numSampleTime = 0;

//         //Trova il massimo sampleTime nella finestra attuale
//         for (const auto& dato : datiFinestre) {
//             numSampleTime = std::max(numSampleTime, dato.sampleTime);
//         }

//         //Debug
//         std::cout << "Massimo Sample Time per il sensore " << sensoreID << ": " << numSampleTime << std::endl;

//         //Itera su ogni possibile inizio di finestra temporale
//         for (int startSampleTime = 0; startSampleTime <= numSampleTime - W; ++startSampleTime) {
//             //inizializza la somma per la finestra attuale
//             double somma = 0.0;

//             //Calcola il sampleTime finale della finestra attuale
//             int endSampleTime = startSampleTime + W - 1;

//             //Debug
//             std::cout << "Start Sample Time: " << startSampleTime << ", End Sample Time: " << endSampleTime << std::endl;

//             //Inizializza il conteggio dei valori nella finestra attuale
//             int conteggio = 0;

//             //Itera su tutti i sampleTime all'interno della finestra attuale 
//             for (int sampleTime = startSampleTime; sampleTime <= endSampleTime; ++sampleTime) {
//                 for (const auto& dato : datiFinestre) {
//                     if (dato.sampleTime == sampleTime) {
//                         somma += dato.value;
//                         ++conteggio;
//                         break;
//                     }
//                 }
//             }
//             if ( conteggio > 0 ) {
//                 //Calcola la media dei valori 
//                 double media = somma / conteggio;

//                 //Debug
//                 std::cout << "SensorID: " << sensoreID << std::endl;
//                 std::cout << "Media per la finestra [" << startSampleTime << ", " << endSampleTime << "]: " << media << std::endl;

//                 //Agginge media al vettore 
//                 mediePerFinestra.emplace_back(startSampleTime, endSampleTime, media);
//             }
//         }
//         //Salva le medie per il sensore corrente
//         mediePerSensore[sensoreID] = mediePerFinestra;
//     }

//     return mediePerSensore;
// }

//Funzione per calcolare le medie su finestre temporali scorrevoloi di lunghezza W
std::map<std::string, std::vector<std::tuple<int, int, double>>> calcolaMedie(const std::map<std::string, std::vector<DatoPerFinestre>>& finestre) {
    std::map<std::string, std::vector<std::tuple<int, int, double>>> mediePerSensore;

    //Itera su ogni finestra temporale
    for (const auto& coppia : finestre) {
        const std::string& sensoreID = coppia.first;
        const std::vector<DatoPerFinestre>& datiFinestre = coppia.second;

        //Deb
        //std::cout << "Elaborazione sensore: " << sensoreID << std::endl;

        //Mappa ordinata per SampleTime con somme parziali
        std::map<int, double> sumMap;

        //Inizializza il numero totale di SampleTime per il sensore
        int numSampleTime = 0;

        //Calcolo somme parziali e ricerca del massimo sampleTime
        for (const auto& dato : datiFinestre) {
            sumMap[dato.sampleTime] += dato.value;
            numSampleTime = std::max(numSampleTime, dato.sampleTime);
        }

        // //Deb
        // for (const auto& pair : sumMap) {
        //     std::cout << "Chiave: " << pair.first << ", Valore: " << pair.second << std::endl;
        // }      

        //Vettore per salvare le medie per ogni finestra
        std::vector<std::tuple<int, int, double>> mediePerFinestra;

        //Itera su ogni possibile inizio di finestra temporale
        for (int startSampleTime = 0; startSampleTime <= numSampleTime - W; ++startSampleTime) {
            //Calcola il sampleTime finale della finestra attuale
            int endSampleTime = startSampleTime + W - 1;

            //Debug
            //std::cout << "Start Sample Time: " << startSampleTime << ", End Sample Time: " << endSampleTime << std::endl;

            //inizializza la somma per la finestra attuale
            double somma = 0.0;

            //Inizializza il conteggio dei valori nella finestra attuale
            int conteggio = 0;

            //Calcolo della somma per la finestra corrente
            auto startIter = sumMap.lower_bound(startSampleTime);
            auto endIter = sumMap.upper_bound(endSampleTime);
            for (auto iter = startIter; iter != endIter; ++iter) {
                somma += iter->second;
                ++conteggio;
            }

            if ( conteggio > 0 ) {
                //Calcola la media dei valori 
                double media = somma / conteggio;

                //Debug
                // std::cout << "SensorID: " << sensoreID << std::endl;
                // std::cout << "Media per la finestra [" << startSampleTime << ", " << endSampleTime << "]: " << media << std::endl;

                //Agginge media al vettore 
                mediePerFinestra.emplace_back(startSampleTime, endSampleTime, media);
            }
        }

        mediePerSensore[sensoreID] = mediePerFinestra;
    }

    return mediePerSensore;
}



