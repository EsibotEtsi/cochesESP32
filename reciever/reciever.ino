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
int angle;
Servo dumbServo1;
Servo dumbServo2;
Servo direction;

#define ENA 16
#define IN1 27
#define IN2 14

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  angle = map(myData.pot1, 0, 4095, 0, 180);
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Potenciometro 1: ");
  Serial.println(myData.pot1);
  Serial.print("Angulo: ");
  Serial.print(angle);

}

void controlMotor(int pot) {
  int speed;
  if(pot>1880){
    speed = map(pot, 1880, 4095, 0, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
  } else if(pot<1850){
    speed = map(pot, 1850, 0, 0, 255);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speed);
  } else {
    speed = 0;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed);
  }
  Serial.print("\tSpeed: ");
  Serial.println(speed);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  dumbServo1.attach(24);
  dumbServo2.attach(25);
  direction.attach(13);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  direction.write(angle);
  controlMotor(myData.pot2);
  delay(50);
}
