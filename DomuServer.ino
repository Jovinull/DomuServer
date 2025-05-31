#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>

// ========== CONFIG WIFI/MQTT ==========
const char* ssid = "FELIPE";
const char* password = "99043425";
const char* mqtt_server = "3a0402e73e714189a5fdf292baf01769.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "jovinull";
const char* mqtt_password = "Secret123";

// === Certificado ISRG Root X1 (Let's Encrypt) ===
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

// ========== PINOS ==========
#define DHTPIN 4
#define DHTTYPE DHT22
#define TRIG_PIN 12
#define ECHO_PIN 13
#define MQ2_A0 32
#define MQ2_D0 25
#define RED_LED 26
#define GREEN_LED 27
#define BUZZER 14

DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE 50
char msg[MSG_BUFFER_SIZE];

unsigned long lastUltrasonicTrigger = 0;
bool objetoDetectado = false;
const int distanciaLimite = 20; // cm
const int tempoDeteccao = 5000; // ms

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi conectado!");

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\n🕒 Horário sincronizado via NTP");

  espClient.setCACert(root_ca);
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("✅ Conectado ao HiveMQ Cloud!");
    } else {
      Serial.print("❌ Falha ao conectar, rc=");
      Serial.print(client.state());
      Serial.println(". Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void enviarDHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h)) {
    Serial.println("💧 Umidade: não detectada");
    client.publish("umidade", "NaN");
  } else {
    Serial.printf("💧 Umidade: %.2f%%\n", h);
    snprintf(msg, MSG_BUFFER_SIZE, "%.2f", h);
    client.publish("umidade", msg);
  }

  if (isnan(t)) {
    Serial.println("🌡 Temperatura: não detectada");
    client.publish("temperatura", "NaN");
  } else {
    Serial.printf("🌡 Temperatura: %.2f°C\n", t);
    snprintf(msg, MSG_BUFFER_SIZE, "%.2f", t);
    client.publish("temperatura", msg);
  }
}

long medirDistanciaCM() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH, 20000); // 20ms timeout
  return duracao * 0.034 / 2;
}

void verificarUltrassonico() {
  long dist = medirDistanciaCM();

  // Se a distância for 0 ou negativa, assume falha (sensor desconectado ou erro)
  if (dist <= 0) {
    Serial.println("📡 Sensor ultrassônico não detectado");
    client.publish("ultrassom", "NaN");
    return;
  }

  Serial.printf("📏 Distância: %ld cm\n", dist);

  if (dist < distanciaLimite) {
    if (!objetoDetectado) {
      objetoDetectado = true;
      lastUltrasonicTrigger = millis();
    } else if (millis() - lastUltrasonicTrigger >= tempoDeteccao) {
      snprintf(msg, MSG_BUFFER_SIZE, "%ld", dist);
      client.publish("ultrassom", msg);
      Serial.println("📤 Publicou distância após detecção contínua");
      objetoDetectado = false;
    }
  } else {
    objetoDetectado = false;
  }
}

void verificarMQ02() {
  int gasAnalog = analogRead(MQ2_A0);
  int gasDigital = digitalRead(MQ2_D0);

  Serial.printf("🧪 Gás MQ-02 (A0): %d | Digital: %d\n", gasAnalog, gasDigital);
  snprintf(msg, MSG_BUFFER_SIZE, "%d", gasAnalog);
  client.publish("gas", msg);

  if (gasDigital == LOW) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    client.publish("gas_alerta", "ALTO");
    Serial.println("⚠️ ALERTA DE GÁS!");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    client.publish("gas_alerta", "normal");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MQ2_D0, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  enviarDHT();
  verificarUltrassonico();
  verificarMQ02();

  delay(1000);
}
