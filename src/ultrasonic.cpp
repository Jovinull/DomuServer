#include "ultrasonic.h"
#include "mqtt_handler.h" // Para usar publishMessage()

// Define os pinos conectados ao sensor HC-SR04
#define TRIG_PIN 12  // Pino de disparo do pulso ultrassônico
#define ECHO_PIN 13  // Pino de recepção do eco

// Parâmetros da lógica de detecção
const int distanciaLimite = 20;        // Distância mínima (em cm) para considerar um objeto próximo
const int tempoDeteccao = 5000;        // Tempo mínimo (ms) que o objeto deve permanecer detectado para envio

// Variáveis de controle do tempo e estado
unsigned long lastUltrasonic = 0;      // Última leitura realizada
const unsigned long intervalUltrasonic = 500; // Intervalo entre leituras
unsigned long triggerStart = 0;        // Tempo de início da detecção de objeto
bool objetoDetectado = false;          // Estado da detecção atual

/**
 * Configura os pinos do sensor ultrassônico.
 * Deve ser chamado uma vez no setup().
 */
void setupUltrasonic() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

/**
 * Dispara um pulso no sensor ultrassônico e mede o tempo de resposta do eco.
 * Retorna a distância em centímetros.
 */
long medirDistanciaCM() {
  digitalWrite(TRIG_PIN, LOW); 
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Mede o tempo de resposta até o eco (timeout de 20ms = 20000us)
  long duracao = pulseIn(ECHO_PIN, HIGH, 20000);
  return duracao * 0.034 / 2; // Converte tempo em microsegundos para distância em cm
}

/**
 * Executa a lógica de leitura do sensor e envio por MQTT.
 * Usa uma máquina de estados baseada em `millis()` para controlar tempo e evitar uso de `delay()`.
 */
void loopUltrasonic() {
  if (millis() - lastUltrasonic >= intervalUltrasonic) {
    lastUltrasonic = millis();

    long dist = medirDistanciaCM();

    // Se valor inválido (erro na leitura), publica NaN e encerra
    if (dist <= 0) {
      publishMessage("ultrassom", "NaN");
      return;
    }

    // Caso a distância esteja abaixo do limite
    if (dist < distanciaLimite) {
      // Primeira detecção
      if (!objetoDetectado) {
        objetoDetectado = true;
        triggerStart = millis();
      }
      // Se objeto permanecer detectado pelo tempo mínimo necessário
      else if (millis() - triggerStart >= tempoDeteccao) {
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%ld", dist);
        publishMessage("ultrassom", buffer);
        objetoDetectado = false; // Reinicia o estado para evitar publicações contínuas
      }
    } else {
      // Distância voltou ao normal, reseta detecção
      objetoDetectado = false;
    }
  }
}
