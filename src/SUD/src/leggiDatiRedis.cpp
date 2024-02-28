#include "main.h"

//Funzione che legge tutti i stream dal Redis e raggrupparli per sensore
std::map<std::string, std::vector<Dato>> leggiDatiRedis(redisContext *context, const std::vector<std::string>& sensori) {
    std::map<std::string, std::vector<Dato>> dati; 

    for (const auto& sensore : sensori) {
        //legge tutti i messaggi dello stream del sensore corrente
        std::string comando = "XRANGE " + sensore + " - +";
        redisReply *reply = (redisReply *)redisCommand(context, comando.c_str());

        //debug
        // std::cout << "Comando Redis: " << comando << std::endl;
        // std::cout << "Risposta Redis: " << (reply ? "valida" : "NULL") <<std::endl;

        if (reply != NULL && reply->type == REDIS_REPLY_ARRAY) {

            for (size_t i = 0; i < reply->elements; ++i){
                redisReply *messaggioReply = reply->element[i];
                Dato nuovoDato;
                //Verifica che il messaggio sia un array
                if (messaggioReply->type == REDIS_REPLY_ARRAY ) {
                    for (size_t k = 0; k < messaggioReply->elements; ++k){
                        redisReply *messaggioDati = messaggioReply->element[k];
                        //Deve prendere array con copie chiave valore
                        if (messaggioDati->type == REDIS_REPLY_ARRAY){
                            //Itera attraverso le coppie chiave-valore
                            for (size_t j = 0; j < messaggioDati->elements; j +=2) {
                                //Verifica che ci siano almeno due elementi nella coppia chiave-valore
                                if (j + 1 < messaggioDati->elements) {
                                    //Estrai chiave e valore
                                    std::string chiave = messaggioDati->element[j]->str;
                                    std::string valore = messaggioDati->element[j + 1]->str;

                                    //std::cout << "Testo di chiave: " << chiave << " Valore: " << valore << std::endl;

                                    if ( chiave == "sampleTime"){
                                        nuovoDato.sampleTime = valore;
                                    } else if ( chiave == "value" ){
                                        nuovoDato.value = valore;
                                    }
                                } else {
                                    std::cerr << "Il messaggio non ha abbastanza elementi." << std::endl;
                                }
                            }
                        }
                    }
                    nuovoDato.SensorID = sensore;
                    dati[sensore].push_back(nuovoDato);
                } else {
                    std::cerr << "Messaggio non valido ricevuto da Redis per il sensore " << sensore << std::endl;
                }
            }
        } else {
            std::cerr << "Errore nella lettura della stream da Redis per il sensore " << sensore << std::endl;
        }
        freeReplyObject(reply);
    }
    return dati;
}