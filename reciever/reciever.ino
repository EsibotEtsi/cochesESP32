//Reciever
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int pot1;
  int pot2;
} struct_message;

Servo direction;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Potenciometro 1: ");
  Serial.println(myData.pot1);
  Serial.print("Potenciometro 2: ");
  Serial.println(myData.pot2);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  direction.attach(14);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

}
 
void loop() {
  direction.write(map(myData.pot2, 0, 4095, 0, 180));
  delay(200);
}
