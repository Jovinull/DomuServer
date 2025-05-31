#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>

// Cliente MQTT exportado globalmente para uso por outros módulos (ex: sensores)
extern PubSubClient client;

/**
 * Inicializa o Wi-Fi, sincroniza o horário via NTP, configura conexão segura com o broker MQTT.
 * Deve ser chamada uma única vez no setup.
 */
void setupMQTT();

/**
 * Mantém a conexão MQTT ativa e processa mensagens pendentes.
 * Deve ser chamada em cada iteração do loop principal.
 */
void mqttLoop();

/**
 * Publica uma mensagem em um tópico MQTT.
 * @param topic Nome do tópico (ex: "temperatura")
 * @param payload Mensagem a ser enviada (em formato texto)
 */
void publishMessage(const char* topic, const char* payload);

#endif
