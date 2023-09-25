#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>


String JSONData;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(16, 17);  // RX, TX - ajuste os pinos conforme necessário

String horaAnterior = "";

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

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Serial.println("bom dia!");
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
        String hora = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
        if (hora != horaAnterior) {
          Serial.println("--------------------");
          Serial.print("Latitude: ");
          Serial.println(latitude, 5);
          Serial.print("Longitude: ");
          Serial.println(longitude, 5);
          Serial.print("Hora: ");
          Serial.println(hora);
          JSONData = toJSON(latitude, longitude, hora);
          Serial.println(JSONData);
        }

        horaAnterior = hora;
      }
    }
  }
}

