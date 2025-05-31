#include "gas_sensor.h"
#include "mqtt_handler.h"

#define MQ2_A0 32
#define MQ2_D0 25
#define RED_LED 26
#define GREEN_LED 27
#define BUZZER 14

void setupGas() {
  pinMode(MQ2_D0, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loopGas() {
  int gasAnalog = analogRead(MQ2_A0);
  int gasDigital = digitalRead(MQ2_D0);

  char buffer[16];  // Agora variável local
  snprintf(buffer, sizeof(buffer), "%d", gasAnalog);
  publishMessage("gas", buffer);

  if (gasDigital == LOW) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    publishMessage("gas_alerta", "ALTO");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    publishMessage("gas_alerta", "normal");
  }
}
