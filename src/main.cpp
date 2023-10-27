#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wifi.h>

// WiFi -----------------------------
const char* ssid       = "UTFPR-IoT";
const char* password   = "IoT@UTFPR-PG.2021";
uint8_t newMACAddress[] = {0xEC, 0xC2, 0x3A, 0x7E, 0xE3, 0x50};
const char* serverAddress = "https://getpost.onrender.com/api/data";



String JSONData;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(16, 17);  

String horaAnterior = "";


void wifiSetup() {
  WiFi.mode(WIFI_STA);
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  Serial.print("Conectando à rede WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado à rede WiFi!");
}


String toJSON(float latitude, float longitude, String hora) {
    StaticJsonDocument<200> doc;
    doc["latitude"] = latitude;
    doc["longitude"] = longitude;
    doc["hora"] = hora;

    // Serializar o JSON em uma string
    String jsonStr;
    serializeJson(doc, jsonStr);
    return jsonStr;
}

void postData(String data) {
  HTTPClient http;
  http.begin(serverAddress);
  // Define os cabeçalhos HTTP para indicar que você está enviando JSON
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(data);
  if (httpResponseCode > 0) {
    Serial.println(data);
    Serial.print("Resposta do servidor: ");
    Serial.println(http.getString());
  } else {
    Serial.print("Falha na solicitação. Código de erro HTTP: ");
    Serial.println(httpResponseCode);
    Serial.print("Erro: ");
    Serial.println(http.errorToString(httpResponseCode).c_str());
  }
  http.end();
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  wifiSetup();
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
        String hora = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
        if (hora != horaAnterior) {
          JSONData = toJSON(latitude, longitude, hora);
          postData(JSONData);
        }

        horaAnterior = hora;
      }
    }
  }
}

