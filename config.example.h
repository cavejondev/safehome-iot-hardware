#pragma once

// Copie este arquivo para config.h e ajuste os valores locais.
// config.h e ignorado pelo Git.
const char* WIFI_SSID = "NOME_DA_REDE_2_4_GHZ";
const char* WIFI_PASSWORD = "SENHA_DA_REDE";

// IP do computador que executa a API na mesma rede do ESP8266.
const char* API_BASE_URL = "http://192.168.137.1:3333/api/v1";

// Token retornado ao cadastrar ou rotacionar o gateway na API.
const char* GATEWAY_TOKEN = "TOKEN_DO_GATEWAY";

// Deve ser igual ao externalId do sensor cadastrado na API.
const char* SENSOR_EXTERNAL_ID = "pir-sala";
