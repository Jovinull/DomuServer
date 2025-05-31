#include <Arduino.h>
#include "src/mqtt_handler.h"
#include "src/dht_sensor.h"
#include "src/ultrasonic.h"
#include "src/gas_sensor.h"

void setup() {
  Serial.begin(115200);

  setupDHT();
  setupUltrasonic();
  setupGas();
  setupMQTT(); // também configura Wi-Fi e tempo
}

void loop() {
  mqttLoop();         // mantém conexão viva
  loopDHT();          // leitura periódica do DHT
  loopUltrasonic();   // leitura do sensor ultrassônico
  loopGas();          // leitura do sensor de gás
}
