# Plano e resultados de testes - SafeHome IoT Hardware

## Identificacao

| Campo                | Preenchimento                    |
| -------------------- | -------------------------------- |
| Versao avaliada      | 1.0.1                            |
| Placa                | Wemos D1 mini baseada no ESP8266 |
| Sensor               | HC-SR04                          |
| Responsavel          | Gabriel Rodrigo Cavejon          |
| Data do teste manual | 11/06/2026                       |

## Objetivo

Validar a compilacao do firmware, leitura do sensor ultrassonico, sinalizacao
pelo LED e comunicacao da Wemos com a API SafeHome.

## Teste de compilacao

Compile o arquivo `fontes/fontes.ino` na Arduino IDE usando a placa
`LOLIN(WEMOS) D1 R2 & mini`.

| Verificacao                | Resultado obtido                            | Status |
| -------------------------- | ------------------------------------------- | ------ |
| Compilacao do firmware     | Compilacao concluida sem erros              | OK     |
| Memoria RAM                | 29.180 de 80.192 bytes utilizados (36%)     | OK     |
| Memoria de instrucoes IRAM | 60.363 de 65.536 bytes utilizados (92%)     | OK     |
| Codigo armazenado em flash | 257.780 de 1.048.576 bytes utilizados (24%) | OK     |

## Teste funcional em uma unica sessao

Abra o Monitor Serial em `9600 baud` e execute os passos abaixo.

| ID    | Acao                                 | Resultado esperado                                     | Resultado obtido | Status |
| ----- | ------------------------------------ | ------------------------------------------------------ | ---------------- | ------ |
| TH-01 | Ligar ou reiniciar a Wemos           | Monitor Serial exibe inicio e firmware `1.0.1`         | Iniciado         | OK     |
| TH-02 | Aguardar conexao com a rede          | Monitor Serial informa Wi-Fi conectado e endereco IP   | Teste OK         | OK     |
| TH-03 | Aguardar aproximadamente 30 segundos | Heartbeat retorna codigo HTTP de sucesso               | Conectado a API  | OK     |
| TH-04 | Aproximar objeto a menos de 60 cm    | LED acende e evento de presenca retorna sucesso        | Led está OK      | OK     |
| TH-05 | Afastar objeto para mais de 60 cm    | LED apaga e leitura indica ausencia                    | Led apagou       | OK     |
| TH-06 | Desligar e religar o Wi-Fi           | Wemos informa falha e tenta reconectar automaticamente | Teste OK         | OK     |

## Teste de carga

Teste de carga concorrente nao se aplica diretamente a uma unica placa Wemos.
O firmware possui intervalo de 30 segundos entre heartbeats e de 5 segundos
entre eventos para evitar excesso de requisicoes. A capacidade de receber dados
de varias placas simultaneamente deve ser testada na API.

## Resumo

| Campo                       | Preenchimento   |
| --------------------------- | --------------- |
| Compilacao                  | APROVADA        |
| Testes funcionais aprovados | APROVADA        |
| Falhas encontradas          | NENHUMA         |
| Resultado final             | TESTES TODOS OK |
