#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Inicializa os pinos e variáveis necessárias para o sensor ultrassônico
void setupUltrasonic();

// Executa periodicamente a leitura do sensor ultrassônico e publica os dados via MQTT
void loopUltrasonic();

#endif
