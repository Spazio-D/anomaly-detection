#include "main.h"

//Funzione per creare finestre temporali
std::map<std::string, std::vector<DatoPerFinestre>> creaFinestre(const std::map<std::string, std::vector<Dato>>& datiPerSensore) {
    std::map<std::string, std::vector<DatoPerFinestre>> finestre;

    for (const auto& coppia : datiPerSensore) {
        const std::string& sensorID = coppia.first;
        const std::vector<Dato>& dati = coppia.second;

        for (const Dato& dato: dati) {
            if (dato.value != "NULL") {
                int intSampleTime = std::stoi(dato.sampleTime);
                double valore = std::stod(dato.value);

                //Crea un oggetto DatoPerFinestre
                DatoPerFinestre datoFinestre;
                datoFinestre.sampleTime = intSampleTime;
                datoFinestre.SensorID = sensorID;
                datoFinestre.value = valore;

                //Aggiunta dell'oggetto alla mappa delle finestre
                finestre[sensorID].push_back(datoFinestre);
            }
        }
    }
    return finestre;
}