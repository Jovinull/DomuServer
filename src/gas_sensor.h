#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

/**
 * Configura os pinos usados pelo sensor MQ-02, LEDs e buzzer.
 * Deve ser chamada uma vez no setup().
 */
void setupGas();

/**
 * Executa a leitura do sensor de gás MQ-02 (analógica e digital),
 * atualiza LEDs e buzzer conforme detecção, e publica os dados via MQTT.
 */
void loopGas();

#endif
