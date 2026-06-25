# SafeHome IoT Hardware

Firmware do hardware do **SafeHome IoT**, desenvolvido para Wemos D1 mini com
ESP8266.

O dispositivo mede distancia com um sensor ultrassonico, interpreta presenca,
acende um LED local e envia eventos para a API SafeHome.

## O que este repositorio faz

- Conecta a Wemos D1 mini em uma rede Wi-Fi 2.4 GHz.
- Mede distancia usando sensor ultrassonico HC-SR04.
- Considera presenca quando a distancia e menor ou igual a 60 cm.
- Acende um LED quando detecta presenca.
- Envia heartbeat periodico para a API.
- Envia evento de presenca quando detecta uma nova ativacao.
- Autentica as chamadas com o header `x-gateway-token`.

## Como o hardware funciona

1. A placa inicia e conecta no Wi-Fi configurado.
2. O firmware mede a distancia usando os pinos `TRIG` e `ECHO`.
3. Se a distancia estiver dentro do limite, considera presenca detectada.
4. O LED liga enquanto existe presenca.
5. Quando a presenca aparece pela primeira vez, o firmware envia um evento para
   a API.
6. A cada 30 segundos, envia um heartbeat informando que o sensor esta online.
7. A API grava os dados e o app mobile exibe os status.

## Requisitos

### Hardware

- Wemos D1 mini ou placa compativel com ESP8266.
- Sensor ultrassonico HC-SR04.
- LED.
- Resistor para o LED.
- Jumpers.
- Cabo USB para gravar a placa.
- Divisor de tensao ou conversor de nivel logico para o pino `ECHO`.

Importante: o ESP8266 trabalha com 3.3 V nos pinos. O `ECHO` do HC-SR04 pode
retornar 5 V. Use divisor de tensao ou conversor de nivel logico para nao
danificar a placa.

### Software

- Arduino IDE.
- Suporte a placas ESP8266 instalado na Arduino IDE.
- Driver USB da placa, se o computador nao reconhecer a Wemos.
- API SafeHome rodando na mesma rede.

## Ligacoes

| Componente | Pino na Wemos |
| --- | --- |
| HC-SR04 TRIG | D5 |
| HC-SR04 ECHO | D6 |
| LED vermelho | D7 |
| GND dos componentes | GND |
| VCC do sensor | 5V |

## Configuracao da Arduino IDE

1. Abra a Arduino IDE.
2. Va em `File > Preferences`.
3. Em `Additional Boards Manager URLs`, adicione:

```text
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

4. Va em `Tools > Board > Boards Manager`.
5. Pesquise por `esp8266`.
6. Instale o pacote `esp8266 by ESP8266 Community`.
7. Selecione a placa:

```text
LOLIN(WEMOS) D1 R2 & mini
```

## Configuracao no Windows

1. Instale a Arduino IDE.
2. Instale o pacote ESP8266 conforme a secao anterior.
3. Conecte a Wemos no USB.
4. Se a porta nao aparecer, instale o driver USB da sua placa.
5. Abra `fontes/fontes.ino`.
6. Copie o arquivo de configuracao:

```powershell
Copy-Item fontes\config.example.h fontes\config.h
```

7. Edite `fontes/config.h`.
8. Selecione placa e porta em `Tools`.
9. Clique em Upload.
10. Abra o Serial Monitor em `9600`.

## Configuracao no Linux

1. Instale a Arduino IDE.
2. Instale o pacote ESP8266 conforme a secao anterior.
3. Conecte a Wemos no USB.
4. Copie o arquivo de configuracao:

```bash
cp fontes/config.example.h fontes/config.h
```

5. Edite `fontes/config.h`.
6. Se a porta serial nao tiver permissao, adicione seu usuario ao grupo correto:

```bash
sudo usermod -aG dialout $USER
```

Depois saia e entre de novo na sessao do Linux.

7. Selecione placa e porta em `Tools`.
8. Clique em Upload.
9. Abra o Serial Monitor em `9600`.

## Arquivo de configuracao

O arquivo real `fontes/config.h` nao deve ir para o Git, porque contem senha de
Wi-Fi e token.

Exemplo:

```cpp
const char* WIFI_SSID = "NOME_DA_REDE_2_4_GHZ";
const char* WIFI_PASSWORD = "SENHA_DA_REDE";
const char* API_BASE_URL = "http://192.168.0.25:3333/api/v1";
const char* GATEWAY_TOKEN = "TOKEN_DO_GATEWAY";
const char* SENSOR_EXTERNAL_ID = "pir-sala";
```

Campos:

- `WIFI_SSID`: nome da rede Wi-Fi.
- `WIFI_PASSWORD`: senha da rede Wi-Fi.
- `API_BASE_URL`: URL da API, usando o IP do computador.
- `GATEWAY_TOKEN`: token retornado pela API ao cadastrar o gateway.
- `SENSOR_EXTERNAL_ID`: identificador do sensor cadastrado na API.

## Como obter o token do gateway

1. Rode a API.
2. Rode o mobile.
3. Entre com um usuario.
4. Crie ou selecione uma residencia.
5. Cadastre um gateway.
6. Copie o token retornado.
7. Cole esse token em `GATEWAY_TOKEN`.

Se estiver usando o seed da API, o token demo e:

```text
SAFEHOME-DEMO-GATEWAY-TOKEN
```

## Como descobrir o IP da API

O hardware nao deve usar `localhost`, porque `localhost` seria a propria Wemos.
Use o IP do computador que esta rodando a API.

### Windows

```powershell
ipconfig
```

Procure o IPv4 da rede Wi-Fi.

### Linux

```bash
hostname -I
```

Exemplo no `config.h`:

```cpp
const char* API_BASE_URL = "http://192.168.0.25:3333/api/v1";
```

## Endpoints usados pelo firmware

Todas as chamadas usam:

```text
x-gateway-token: TOKEN_DO_GATEWAY
```

### Heartbeat

```text
POST /api/v1/iot/heartbeats
```

Payload enviado:

```json
{
  "sensors": [
    {
      "externalId": "pir-sala"
    }
  ],
  "helpButtons": []
}
```

### Evento de presenca

```text
POST /api/v1/iot/presence-events
```

Payload enviado:

```json
{
  "sensorExternalId": "pir-sala"
}
```

## Ordem recomendada para testar tudo

1. Suba o banco da API.
2. Rode migrations e seed.
3. Inicie a API.
4. Teste `/api/v1/health`.
5. Configure o `config.h` com Wi-Fi, IP da API, token e sensor.
6. Grave o firmware na Wemos.
7. Abra o Serial Monitor.
8. Veja se aparece conexao Wi-Fi.
9. Veja se aparecem respostas HTTP com codigo 2xx.
10. Abra o app mobile e confira os eventos.

## Mensagens esperadas no Serial Monitor

Ao iniciar:

```text
[SAFE HOME IOT INICIADO]
[FIRMWARE] Versao 1.0.1
[WIFI] Conectado. IP da Wemos: ...
```

Quando envia dados:

```text
[HTTP] POST http://.../api/v1/iot/heartbeats
[HTTP] Codigo: 200
[HEARTBEAT] Enviado com sucesso.
```

Quando detecta presenca:

```text
[PRESENCA] Presenca detectada pelo ultrassonico.
[PRESENCA] Evento enviado com sucesso.
```

## Problemas comuns

### A placa nao aparece na Arduino IDE

- Troque o cabo USB.
- Instale o driver USB correto da placa.
- No Linux, confira permissao do usuario no grupo `dialout`.

### Nao conecta no Wi-Fi

- Use rede 2.4 GHz.
- Confira nome e senha.
- Evite redes com portal de login.
- Deixe a placa perto do roteador para testar.

### HTTP retorna erro 401

O token do gateway esta errado.

Confira `GATEWAY_TOKEN` em `config.h` e o token cadastrado na API.

### HTTP nao conecta

Verifique:

- a API esta rodando;
- a Wemos e o computador estao na mesma rede;
- `API_BASE_URL` usa IP do computador, nao `localhost`;
- firewall do computador permite conexao na porta `3333`;
- a URL termina com `/api/v1`.

### Sensor sempre sem resposta

- Confira `TRIG` em D5 e `ECHO` em D6.
- Confira alimentacao do HC-SR04.
- Confira GND comum entre sensor e Wemos.
- Use divisor de tensao no `ECHO`.

## Testes

O plano de testes fica em:

```text
tests/PLANO_E_RESULTADOS.md
```

Use essa pasta para guardar evidencias de compilacao, upload, Serial Monitor e
funcionamento integrado com a API.

## Licenca

Projeto sob licenca MIT. Veja `LICENSE` e `THIRD_PARTY_NOTICES.md`.