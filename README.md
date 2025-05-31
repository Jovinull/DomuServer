# DomuServer – Sistema IoT com ESP32, Sensores e MQTT

Este repositório contém a implementação completa do projeto **DomuServer**, que integra múltiplos sensores ao ESP32 com comunicação via **MQTT sobre TLS** usando o broker **HiveMQ Cloud**. O projeto utiliza uma **máquina de estados com `millis()`** para evitar bloqueios com `delay()`, garantindo desempenho assíncrono e escalável.

## Objetivos

* Monitorar temperatura, umidade, gás inflamável e distância de objetos em tempo real.
* Utilizar o protocolo MQTT com TLS (porta 8883) para comunicação segura.
* Publicar os dados em tópicos organizados e acessíveis para automações ou dashboards.
* Garantir fluidez de execução com controle assíncrono por `millis()` (sem `delay()`).
* Modularizar o código em arquivos separados para manutenção facilitada.

---

## Arquitetura do Sistema

A arquitetura foi estruturada com os seguintes módulos:

| Módulo         | Responsabilidade                                                               |
| -------------- | ------------------------------------------------------------------------------ |
| `mqtt_handler` | Conexão Wi-Fi, configuração TLS, conexão e publicação via MQTT (HiveMQ Cloud). |
| `dht_sensor`   | Leitura de temperatura e umidade via DHT22 a cada 5s.                          |
| `ultrasonic`   | Medição de distância via HC-SR04 e publicação se o objeto persistir por 5s.    |
| `gas_sensor`   | Monitoramento de gás inflamável com alerta visual/sonoro e envio via MQTT.     |

Cada módulo possui controle próprio de tempo baseado em `millis()`.

---

## Tópicos MQTT Utilizados

| Tópico        | Origem            | Tipo de dado     | Exemplo de valor       | Descrição                                    |
| ------------- | ----------------- | ---------------- | ---------------------- | -------------------------------------------- |
| `temperatura` | Sensor DHT22      | `float` (string) | `"28.56"`              | Temperatura em °C                            |
| `umidade`     | Sensor DHT22      | `float` (string) | `"62.17"`              | Umidade relativa em %                        |
| `ultrassom`   | Sensor HC-SR04    | `long` (string)  | `"18"`                 | Distância em cm, após presença contínua > 5s |
| `gas`         | Sensor MQ-02 (A0) | `int` (string)   | `"821"`                | Leitura analógica da concentração de gás     |
| `gas_alerta`  | Sensor MQ-02 (D0) | `string`         | `"normal"` ou `"ALTO"` | Estado do alerta de gás                      |

---

## Componentes Utilizados

| Componente                    | Modelo         | Função                                 |
| ----------------------------- | -------------- | -------------------------------------- |
| Microcontrolador              | DOIT ESP32 V1  | Unidade de controle e comunicação MQTT |
| Sensor de Temperatura/Umidade | DHT22          | Mede temperatura e umidade do ambiente |
| Sensor Ultrassônico           | HC-SR04        | Detecta distância de objetos           |
| Sensor de Gás                 | MQ-02          | Detecta presença de gases inflamáveis  |
| LEDs                          | Vermelho/Verde | Indicadores visuais de estado do gás   |
| Buzzer                        | Passivo        | Alerta sonoro de perigo                |

---

## Configuração Inicial

### 1. Ajustes de Wi-Fi e MQTT

No arquivo `mqtt_handler.cpp`, configure:

```cpp
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";
const char* mqtt_server = "seu-endereco-broker.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "SEU_USUARIO";
const char* mqtt_password = "SUA_SENHA";
```

> Este projeto utiliza **MQTT com TLS (porta 8883)** e exige o certificado da Let's Encrypt (ISRG Root X1), já incluído no projeto.

---

## Fluxo de Execução

* A cada 5 segundos: temperatura e umidade são lidas e publicadas.
* A cada 1 segundo: distância é medida. Se for menor que 20 cm por mais de 5s, publica no tópico `ultrassom`.
* A cada 1 segundo: concentração de gás é medida. Se o sinal digital for ativado (gás detectado), o LED vermelho e o buzzer são ativados.

---

## Como Usar

1. Conecte os sensores aos pinos conforme definido nos `.h`.
2. Compile e envie o projeto ao ESP32 (IDE Arduino ou PlatformIO).
3. Monitore os tópicos MQTT com seu cliente preferido (ex: MQTT Explorer, Node-RED).
4. Configure automações baseadas nos tópicos.

---

## Estrutura de Arquivos

```plaintext
domuserve.ino               <- Loop principal e setup geral
/src/
├── mqtt_handler.h/.cpp     <- Conexão Wi-Fi, MQTT, publicação segura
├── dht_sensor.h/.cpp       <- Sensor DHT22 com controle por millis()
├── ultrasonic.h/.cpp       <- Sensor ultrassônico com lógica de persistência
├── gas_sensor.h/.cpp       <- Sensor MQ-02 com LEDs e buzzer
```

---

## Resultados Esperados

* Redução do uso de `delay()` para fluidez total do código.
* Modularização clara e expansível para novos sensores.
* Comunicação MQTT segura e assíncrona com HiveMQ Cloud.
* Comportamentos visuais e sonoros em tempo real.

---

## Considerações Finais

Este projeto é um exemplo sólido de como implementar uma **arquitetura robusta e escalável com ESP32**, utilizando uma **máquina de estados baseada em tempo real (`millis()`)**. A separação em módulos permite fácil manutenção, testes individuais e evolução futura.

---

## Links Úteis

| Categoria       | Recurso                                                                                               |
| --------------- | ----------------------------------------------------------------------------------------------------- |
| MQTT            | [MQTT.org](https://mqtt.org) • [HiveMQ Cloud](https://console.hivemq.cloud/)                          |
| ESP32           | [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) |
| Arduino         | [Arduino IDE v2](https://www.arduino.cc/en/software)                                                  |
| Tutoriais úteis | [MQTT com ESP32](https://www.youtube.com/watch?v=aqLq-22A5rU)                                         |
| Sensores        | [HC-SR04 com Arduino](https://www.makerhero.com/blog/sensor-ultrassonico-hc-sr04-ao-arduino/)         |
