#include <TinyGPS++.h>
#include <SoftwareSerial.h>


TinyGPSPlus gps;
SoftwareSerial gpsSerial(16, 17);  // RX, TX - ajuste os pinos conforme necessário

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
        
        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
        Serial.print("Hora: ");
        Serial.println(hora);

        delay(1000);
        // Aqui você pode publicar as coordenadas e a hora como desejar.
      }
    }
  }
}