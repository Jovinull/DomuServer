#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

/**
 * Inicializa o sensor DHT22 configurando o pino e preparando para leitura.
 * Deve ser chamada no setup principal.
 */
void setupDHT();

/**
 * Executa leituras periódicas do sensor de temperatura e umidade.
 * Os dados são enviados via MQTT nos tópicos apropriados.
 * Controlado por máquina de estados com `millis()`.
 */
void loopDHT();

#endif
