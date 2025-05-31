#include "gas_sensor.h"
#include "mqtt_handler.h" // Para usar publishMessage()

// ========================
// Mapeamento dos pinos
// ========================

#define MQ2_A0 32          // Pino analógico do sensor MQ-02 (saída de concentração de gás)
#define MQ2_D0 25          // Pino digital do sensor MQ-02 (nível lógico baseado em limite interno)

#define RED_LED 26         // LED vermelho: indica perigo (gás detectado)
#define GREEN_LED 27       // LED verde: ambiente seguro
#define BUZZER 14          // Buzzer: alarme sonoro em caso de gás detectado

/**
 * Inicializa os pinos do sensor MQ-02 e os atuadores (LEDs e buzzer).
 * Define os pinos como entrada/saída conforme necessário.
 */
void setupGas() {
  pinMode(MQ2_D0, INPUT);     // Entrada digital do sensor
  pinMode(RED_LED, OUTPUT);   // Saída para LED vermelho
  pinMode(GREEN_LED, OUTPUT); // Saída para LED verde
  pinMode(BUZZER, OUTPUT);    // Saída para buzzer
}

/**
 * Executa a leitura do sensor MQ-02 e aciona os indicadores de alerta.
 * Publica a leitura analógica (concentração de gás) e estado de alerta (digital).
 */
void loopGas() {
  // Leitura da saída analógica do sensor (concentração de gás)
  int gasAnalog = analogRead(MQ2_A0);

  // Leitura da saída digital (nível lógico alto ou baixo)
  int gasDigital = digitalRead(MQ2_D0);

  // Publica a concentração analógica no tópico "gas"
  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d", gasAnalog);
  publishMessage("gas", buffer);

  // Se o digital estiver em nível baixo (gás detectado), ativa alerta
  if (gasDigital == LOW) {
    digitalWrite(RED_LED, HIGH);       // Liga LED vermelho
    digitalWrite(GREEN_LED, LOW);      // Desliga LED verde
    digitalWrite(BUZZER, HIGH);        // Liga o buzzer
    publishMessage("gas_alerta", "ALTO");
  } 
  // Caso contrário, ambiente está seguro
  else {
    digitalWrite(RED_LED, LOW);        // Desliga LED vermelho
    digitalWrite(GREEN_LED, HIGH);     // Liga LED verde
    digitalWrite(BUZZER, LOW);         // Desliga o buzzer
    publishMessage("gas_alerta", "normal");
  }
}
