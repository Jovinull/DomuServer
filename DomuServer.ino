#include <Arduino.h>

// Importa os módulos customizados do projeto
#include "src/mqtt_handler.h"    // Responsável por conexão Wi-Fi, sincronização NTP e comunicação MQTT
#include "src/dht_sensor.h"      // Responsável pela leitura do sensor de temperatura e umidade (DHT22)
#include "src/ultrasonic.h"      // Responsável pela leitura do sensor de distância ultrassônico (HC-SR04)
#include "src/gas_sensor.h"      // Responsável pela leitura do sensor de gás (MQ-02), controle de LEDs e buzzer

/**
 * Função de inicialização executada uma única vez após o boot ou reset.
 * Aqui configuramos todos os sensores e a comunicação MQTT.
 */
void setup() {
  // Inicializa a comunicação serial com velocidade de 115200 baud para depuração
  Serial.begin(115200);

  // Inicializa o sensor DHT22 (temperatura e umidade)
  setupDHT();

  // Inicializa o sensor ultrassônico (HC-SR04)
  setupUltrasonic();

  // Inicializa o sensor de gás MQ-02, LEDs e buzzer
  setupGas();

  // Inicializa conexão Wi-Fi, sincronização NTP e conexão segura com broker MQTT
  setupMQTT();
}

/**
 * Função executada continuamente após o setup.
 * Aqui é feito o controle da lógica baseada em máquina de estados usando millis().
 */
void loop() {
  // Mantém a conexão MQTT ativa e processa mensagens pendentes
  mqttLoop();

  // Realiza leitura do DHT22 a cada intervalo definido e publica no broker MQTT
  loopDHT();

  // Realiza medição de distância com o HC-SR04 e publica se houver objeto detectado continuamente
  loopUltrasonic();

  // Lê o sensor MQ-02, atualiza LEDs/buzzer e publica os valores no broker MQTT
  loopGas();
}
