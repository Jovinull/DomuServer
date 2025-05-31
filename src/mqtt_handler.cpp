#include "mqtt_handler.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>

// =====================
// Configurações da rede
// =====================

// Nome da rede Wi-Fi
const char* ssid = "FELIPE";

// Senha da rede Wi-Fi
const char* password = "99043425";

// Endereço do broker MQTT (HiveMQ Cloud com TLS)
const char* mqtt_server = "3a0402e73e714189a5fdf292baf01769.s1.eu.hivemq.cloud";

// Porta segura (TLS) para MQTT
const int mqtt_port = 8883;

// Credenciais do broker MQTT
const char* mqtt_user = "jovinull";
const char* mqtt_password = "Secret123";

// ==============================
// Certificado raiz ISRG Root X1
// ==============================
// Necessário para validar a conexão TLS com o HiveMQ Cloud
const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

// ===========================
// Objetos globais do cliente
// ===========================

// Cliente WiFi seguro (TLS)
WiFiClientSecure espClient;

// Cliente MQTT com camada segura
PubSubClient client(espClient);

/**
 * Realiza as seguintes etapas:
 * - Conexão à rede Wi-Fi
 * - Sincronização do horário via NTP (necessária para TLS)
 * - Configuração do certificado raiz e conexão com broker MQTT
 */
void setupMQTT() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // Exceção ao uso de delay: conexão Wi-Fi
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi conectado!");

  // Sincroniza horário (obrigatório para TLS baseado em data/validação de certificado)
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) { // Espera tempo válido (ex: após 1970)
    delay(500);
    now = time(nullptr);
  }
  Serial.println("🕒 Horário sincronizado");

  // Define o certificado para validação da conexão segura com o broker
  espClient.setCACert(root_ca);

  // Configura o servidor MQTT (TLS)
  client.setServer(mqtt_server, mqtt_port);
}

/**
 * Reconecta ao broker MQTT caso a conexão tenha sido perdida.
 * Tenta novamente a cada 5 segundos com novo clientId.
 */
void mqttReconnect() {
  while (!client.connected()) {
    String clientId = "ESP32Client-" + String(random(0xffff), HEX); // Evita conflito de ID

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("✅ Conectado ao MQTT");
    } else {
      Serial.print("❌ Falha MQTT: ");
      Serial.println(client.state());
      delay(5000); // Pequeno delay entre tentativas de reconexão
    }
  }
}

/**
 * Deve ser chamada continuamente no loop principal.
 * - Mantém a conexão ativa com o broker
 * - Processa as mensagens pendentes (keep-alive)
 */
void mqttLoop() {
  if (!client.connected()) mqttReconnect();
  client.loop();
}

/**
 * Envia uma mensagem MQTT no tópico especificado.
 * Pode ser usada por qualquer sensor/módulo.
 * @param topic Nome do tópico (ex: "ultrassom")
 * @param payload Mensagem a ser enviada (ex: "23.5")
 */
void publishMessage(const char* topic, const char* payload) {
  client.publish(topic, payload);
}