# SafeHome IoT Hardware

Firmware para a placa Wemos D1 mini baseada no ESP8266. O dispositivo detecta
presenca com um sensor ultrassonico, envia heartbeats e registra eventos na API
SafeHome.

## Hardware

- Wemos D1 mini baseada no ESP8266
- Sensor ultrassonico HC-SR04
- LED e resistor

### Ligacoes

| Componente | Pino |
| --- | --- |
| HC-SR04 TRIG | D5 |
| HC-SR04 ECHO | D6 |
| LED | D7 |

O pino ECHO do HC-SR04 opera em 5 V. Utilize divisor de tensao ou conversor de
nivel logico antes de conecta-lo ao ESP8266, que opera em 3,3 V.

## Dependencias

Utilize a Arduino IDE apenas como ferramenta de compilacao e gravacao. Instale
nela o suporte a placas ESP8266 e selecione `LOLIN(WEMOS) D1 R2 & mini`.

## Configuracao

1. Copie `config.example.h` para `config.h`.
2. Configure Wi-Fi, endereco da API, token do gateway e `externalId`.
3. Garanta que a Wemos e o computador da API estejam na mesma rede 2,4 GHz.
4. Abra `fontes.ino`, selecione a placa e envie o firmware.

O arquivo `config.h` contem credenciais locais e nao deve ser enviado ao Git.

## API

O firmware utiliza:

- `POST /api/v1/iot/heartbeats`
- `POST /api/v1/iot/presence-events`

Todas as requisicoes incluem o header `x-gateway-token`.

## Testes

O plano de compilacao e os testes funcionais do hardware estao documentados na
pasta `tests`.

## Licenca

O codigo deste projeto esta disponivel sob a licenca MIT. Bibliotecas externas
continuam sujeitas as suas proprias licencas.
