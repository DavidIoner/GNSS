#include <WiFi.h>
#include <esp_wifi.h>

// Set your new MAC Address
uint8_t newMACAddress[] = {0xec, 0xc2, 0x3a, 0x7e, 0xe3, 0x50};

void setup(){
  Serial.begin(9600);
  Serial.println();
  
  WiFi.mode(WIFI_STA);
 
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}

