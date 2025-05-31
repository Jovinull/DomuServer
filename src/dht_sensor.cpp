#include "dht_sensor.h"
#include <DHT.h>
#include "mqtt_handler.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastDHT = 0;
const unsigned long intervalDHT = 5000;

void setupDHT() {
  dht.begin();
}

void loopDHT() {
  unsigned long now = millis();
  if (now - lastDHT >= intervalDHT) {
    lastDHT = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    char buffer[16];  // Agora variável local

    if (!isnan(h)) {
      snprintf(buffer, sizeof(buffer), "%.2f", h);
      publishMessage("umidade", buffer);
    }

    if (!isnan(t)) {
      snprintf(buffer, sizeof(buffer), "%.2f", t);
      publishMessage("temperatura", buffer);
    }
  }
}
