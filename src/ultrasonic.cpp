#include "ultrasonic.h"
#include "mqtt_handler.h"

#define TRIG_PIN 12
#define ECHO_PIN 13

const int distanciaLimite = 20;
const int tempoDeteccao = 5000;

unsigned long lastUltrasonic = 0;
const unsigned long intervalUltrasonic = 500;
unsigned long triggerStart = 0;
bool objetoDetectado = false;

void setupUltrasonic() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

long medirDistanciaCM() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH, 20000);
  return duracao * 0.034 / 2;
}

void loopUltrasonic() {
  if (millis() - lastUltrasonic >= intervalUltrasonic) {
    lastUltrasonic = millis();
    long dist = medirDistanciaCM();

    if (dist <= 0) {
      publishMessage("ultrassom", "NaN");
      return;
    }

    if (dist < distanciaLimite) {
      if (!objetoDetectado) {
        objetoDetectado = true;
        triggerStart = millis();
      } else if (millis() - triggerStart >= tempoDeteccao) {
        char buffer[16];  // Agora variável local
        snprintf(buffer, sizeof(buffer), "%ld", dist);
        publishMessage("ultrassom", buffer);
        objetoDetectado = false;
      }
    } else {
      objetoDetectado = false;
    }
  }
}
