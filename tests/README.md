# Testes e validacao do SafeHome IoT Hardware

Os testes do firmware dependem da placa Wemos D1 mini, do sensor HC-SR04, da
rede Wi-Fi e da API. Por isso, a validacao funcional deve ser realizada
manualmente com o Monitor Serial da Arduino IDE aberto em `9600 baud`.

## Execucao rapida

1. Configure e ligue a API.
2. Conecte a Wemos e abra o Monitor Serial em `9600 baud`.
3. Aproxime e afaste um objeto do sensor HC-SR04.
4. Desligue e ligue novamente a rede Wi-Fi.
5. Marque os resultados em
   [`PLANO_E_RESULTADOS.md`](./PLANO_E_RESULTADOS.md).

Todo o roteiro pode ser executado em uma unica sessao de poucos minutos.

## Evidencias

Caso sejam solicitadas, salve capturas do Monitor Serial na pasta `evidencias`.
