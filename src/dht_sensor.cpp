#include "dht_sensor.h"
#include <DHT.h>             // Biblioteca oficial do sensor DHT
#include "mqtt_handler.h"    // Para uso da função publishMessage()

// =======================
// Configurações do sensor
// =======================

#define DHTPIN 4             // Pino conectado ao sinal de dados do DHT22
#define DHTTYPE DHT22        // Tipo do sensor (DHT11 ou DHT22)

// Instancia o sensor com as configurações definidas
DHT dht(DHTPIN, DHTTYPE);

// Controle de tempo baseado em millis()
unsigned long lastDHT = 0;                // Armazena o tempo da última leitura
const unsigned long intervalDHT = 5000;   // Intervalo entre leituras (em ms)

/**
 * Inicializa o sensor DHT.
 * Deve ser chamada apenas uma vez no setup().
 */
void setupDHT() {
  dht.begin(); // Inicia comunicação com o sensor
}

/**
 * Lê os dados do sensor a cada 5 segundos e publica via MQTT.
 * Usa controle assíncrono com millis() (sem delay).
 */
void loopDHT() {
  unsigned long now = millis();

  // Verifica se já passou o intervalo definido desde a última leitura
  if (now - lastDHT >= intervalDHT) {
    lastDHT = now;

    // Realiza a leitura da umidade e temperatura
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Buffer para formatar os valores como texto (payload MQTT)
    char buffer[16];

    // Verifica e publica a umidade, se válida
    if (!isnan(h)) {
      snprintf(buffer, sizeof(buffer), "%.2f", h);
      publishMessage("umidade", buffer);
    }

    // Verifica e publica a temperatura, se válida
    if (!isnan(t)) {
      snprintf(buffer, sizeof(buffer), "%.2f", t);
      publishMessage("temperatura", buffer);
    }
  }
}
