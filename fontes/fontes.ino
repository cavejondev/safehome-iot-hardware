#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "config.h"

const char* FIRMWARE_VERSION = "1.0.1";

// --- HARDWARE ---
// Wemos D1 mini:
// TRIG -> D5
// ECHO -> D6
// LED vermelho -> D7
const int TRIG_PIN = D5;
const int ECHO_PIN = D6;
const int LED_PIN = D7;
const unsigned int MAX_DISTANCE_CM = 300;
const unsigned long ECHO_TIMEOUT_US = MAX_DISTANCE_CM * 58UL;

// Distancia considerada "presenca detectada".
const float PRESENCE_DISTANCE_CM = 60.0;

// --- INTERVALOS ---
const unsigned long WIFI_RETRY_INTERVAL_MS = 10000;
const unsigned long LOOP_DELAY_MS = 200;
const unsigned long HEARTBEAT_INTERVAL_MS = 30000;
const unsigned long EVENT_COOLDOWN_MS = 5000;
const unsigned long SENSOR_LOG_INTERVAL_MS = 1000;

unsigned long lastHeartbeatAt = HEARTBEAT_INTERVAL_MS;
unsigned long lastPresenceEventAt = 0;
unsigned long lastWifiRetryAt = 0;
unsigned long lastSensorLogAt = 0;
bool lastSensorState = false;

String buildUrl(const char* path) {
  return String(API_BASE_URL) + path;
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (durationUs == 0) {
    return -1.0;
  }

  return durationUs * 0.0343 / 2.0;
}

void connectToWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("[WIFI] Conectando em ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startedAt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startedAt < WIFI_RETRY_INTERVAL_MS) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("[WIFI] Conectado. IP da Wemos: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WIFI] Gateway da rede: ");
    Serial.println(WiFi.gatewayIP());
    return;
  }

  Serial.println();
  Serial.println("[WIFI] Falha ao conectar. Vou tentar de novo.");
}

bool sendJsonPost(const char* path, const String& payload) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HTTP] Sem Wi-Fi para enviar dados.");
    return false;
  }

  WiFiClient client;
  HTTPClient http;
  String url = buildUrl(path);

  Serial.print("[HTTP] POST ");
  Serial.println(url);
  Serial.print("[HTTP] Payload: ");
  Serial.println(payload);

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  http.addHeader("x-gateway-token", GATEWAY_TOKEN);

  int responseCode = http.POST(payload);
  String responseBody = http.getString();

  Serial.print("[HTTP] Codigo: ");
  Serial.println(responseCode);

  if (responseBody.length() > 0) {
    Serial.print("[HTTP] Resposta: ");
    Serial.println(responseBody);
  }

  http.end();
  return responseCode > 0 && responseCode < 300;
}

void sendHeartbeat() {
  String payload = "{";
  payload += "\"sensors\":[{";
  payload += "\"externalId\":\"" + String(SENSOR_EXTERNAL_ID) + "\"";
  payload += "}]";
  payload += ",\"helpButtons\":[]";
  payload += "}";

  bool success = sendJsonPost("/iot/heartbeats", payload);
  if (success) {
    Serial.println("[HEARTBEAT] Enviado com sucesso.");
  } else {
    Serial.println("[HEARTBEAT] Falha ao enviar.");
  }
}

void sendPresenceEvent() {
  String payload = "{";
  payload += "\"sensorExternalId\":\"" + String(SENSOR_EXTERNAL_ID) + "\"";
  payload += "}";

  bool success = sendJsonPost("/iot/presence-events", payload);
  if (success) {
    Serial.println("[PRESENCA] Evento enviado com sucesso.");
  } else {
    Serial.println("[PRESENCA] Falha ao enviar evento.");
  }
}

void ensureWiFiConnected() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  unsigned long now = millis();
  if (now - lastWifiRetryAt < WIFI_RETRY_INTERVAL_MS) {
    return;
  }

  lastWifiRetryAt = now;
  connectToWiFi();
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  delay(1000);
  Serial.println();
  Serial.println("[SAFE HOME IOT INICIADO]");
  Serial.print("[FIRMWARE] Versao ");
  Serial.println(FIRMWARE_VERSION);
  Serial.println("[SENSOR] TRIG em D5 | ECHO em D6");

  WiFi.mode(WIFI_STA);
  connectToWiFi();

  float initialDistance = readDistanceCm();
  lastSensorState = initialDistance > 0 && initialDistance <= PRESENCE_DISTANCE_CM;

  if (initialDistance < 0) {
    Serial.println("[SENSOR] Sem leitura inicial.");
  } else {
    Serial.print("[SENSOR] Leitura inicial: ");
    Serial.print(initialDistance);
    Serial.println(" cm");
  }
}

void loop() {
  ensureWiFiConnected();

  unsigned long now = millis();
  float distanceCm = readDistanceCm();
  bool sensorActive = distanceCm > 0 && distanceCm <= PRESENCE_DISTANCE_CM;

  digitalWrite(LED_PIN, sensorActive ? HIGH : LOW);

  if (now - lastSensorLogAt >= SENSOR_LOG_INTERVAL_MS) {
    if (distanceCm < 0) {
      Serial.println("[SENSOR] Sem resposta.");
    } else {
      Serial.print("[SENSOR] Distancia: ");
      Serial.print(distanceCm);
      Serial.print(" cm | Presenca: ");
      Serial.println(sensorActive ? "SIM" : "NAO");
    }

    lastSensorLogAt = now;
  }

  // Envia evento somente na borda de ativacao para nao floodar a API.
  if (sensorActive && !lastSensorState && now - lastPresenceEventAt >= EVENT_COOLDOWN_MS) {
    Serial.println("[PRESENCA] Presenca detectada pelo ultrassonico.");
    sendPresenceEvent();
    lastPresenceEventAt = now;
  }

  if (now - lastHeartbeatAt >= HEARTBEAT_INTERVAL_MS) {
    sendHeartbeat();
    lastHeartbeatAt = now;
  }

  lastSensorState = sensorActive;
  delay(LOOP_DELAY_MS);
}
