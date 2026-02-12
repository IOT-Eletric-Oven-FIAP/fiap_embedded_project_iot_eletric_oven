# Projeto FIAP - Forno elétrico IOT


## Objetivo
Implementar um **forno IoT** (simulado no Wokwi) com:

- **Medição de temperatura** via sensor NTC
- **Interface local** com **display**
- **Botão liga/desliga** (habilita/desabilita a operação do forno)
- **Potenciômetro** para ajuste do **nível de saída** (PWM) / referência de controle
- **LED** representando a resistência/aquecimento, variando conforme a saída (PWM)

Além disso, o firmware publica telemetria via **MQTT** para consumo em um dashboard (Ubidots).

> **Mudança de escopo:** este projeto **não é mais** uma automação residencial. As funções de **detecção de gás** e **automação de luzes** foram removidas.

---

link do overleaf com a [doc provisória](https://www.overleaf.com/project/69850e0b3ed4867cd1915cac)

links do draw.io:

* [Diagrama Geral](https://drive.google.com/file/d/19P7n8HH2ntk0AozeMG-ax0MThreavw5b/view?usp=sharing)
* [Fluxograma do Sistema](https://app.diagrams.net/#G1lBIXfRnp0AMUoXh9VU9wFyvFj2jhZB6u#%7B%22pageId%22%3A%22SKCNiIy7pvsI1ocINuwh%22%7D)

---


# Projeto prático em grupo — Sistema de **Forno IoT**
Projeto do programa de especialização/residência em sistemas eletrônicos embarcados (parceria MCTI/Softex + CPQD + FIAP)

---

#### Contato
Para dúvidas, comentários ou sugestões:

- [Matheus Grossi](https://www.linkedin.com/in/matheus-grossi/)

- [Talles Mello](https://www.linkedin.com/in/tallesmello/)

- [Manassés Loiola](https://www.linkedin.com/in/manass%C3%A9s-loiola-de-souza-0b213624/)

- [Helton Abadia](https://www.linkedin.com/in/helton-rosa-da-silva-abadia-6177892ab/)

#### Links
- Simulação no Wokwi: https://wokwi.com/projects/454130082230099969
- Dashboard no Ubidots: https://stem.ubidots.com/app/dashboards/69701418221d689174891929



---

## Índice
1. [Descrição do sistema](#1-descrição-do-sistema)
2. [Hardware e lista de materiais](#2-hardware-e-lista-de-materiais)
3. [Mapa de pinos e conexões](#3-mapa-de-pinos-e-conexões)
4. [Comunicação MQTT e telemetria](#4-comunicação-mqtt-e-telemetria)
5. [Arquitetura do firmware](#5-arquitetura-do-firmware)
6. [Integração com Ubidots](#6-integração-com-ubidots)
7. [Próximos passos](#7-próximos-passos)

---

## 1. Descrição do sistema
O sistema simula um **forno com conectividade IoT**. O ESP32:

1. Lê a **temperatura** do NTC.
2. Lê o **potenciômetro** (referência do usuário), que pode ser usado como:
   - **setpoint** (temperatura-alvo), ou
   - **potência/PWM** diretamente (duty cycle).
3. Com o forno habilitado pelo **botão**, calcula a saída e aciona o **LED** (representando a resistência).
4. Publica periodicamente os dados via MQTT (ex.: temperatura, referência, estado do forno e saída).
5. Exibe informações no **display** (a integrar no circuito).

Arquitetura (alto nível):

```text
[NTC + Pot + Botão + Display] -> ESP32 -> Wi-Fi -> Broker MQTT -> Ubidots Dashboard
                                   |
                                   +-> LED (saída / resistência)
```

---

## 2 Lista de materiais utilizados nesta aplicação:

 
<div align="center">

| **Item** | **Imagem** | **Link referêcia** |
| :--- | :---: | :---: |
| **ESP32-S3-DEVKITC-1-N8R8** | ![ESP32-S3-DEVKITC-1-N8R8](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/1.esp32.png) | [Link-1](https://www.digikey.com.br/en/products/detail/espressif-systems/ESP32-S3-DEVKITC-1-N8R8/15295894) |
| **Display LCD 20x4** | ![Display LCD 20x4](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/2.display.jpg) | [Link-2](https://www.usinainfo.com.br/display-arduino/display-lcd-20x4-com-fundo-azul-2727.html?srsltid=AfmBOoo_T0yx8DeO8kDobWg-LXnzvC8cdNzDUk0HhwmnHMre7m4wweJD) |
| **Sensor de temperatura NTC-10K** | ![Sensor de temperatura NTC-10K](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/3.sensor_de_temperatura.png) | [Link-3](https://www.arducore.com.br/modulo-sensor-de-temperatura-ntc?srsltid=AfmBOoq0kWVqQLXZ2cuCdpVPkEVKR5jUI-C0HkQP3z2ZFlozv6QAMyRD) |
| **Push-Button** | ![Push-Button](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/4.push_button.png) | [Link-4](https://www.eletrogate.com/push-button-chave-tactil-6x6x6mm?srsltid=AfmBOop5mCOiH2MCIOyZxYxCbOE5eBJbDxK-dSsRCbRR-rpGESQx8RGx) |
| **Led vermelho** | ![Led vermelho](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/5.led_vermelho.png) | [Link-5](https://www.mercadolivre.com.br/kit-1000-leds-difusos-lamp-5mm-vermelhos-para-projetos-eletrnicos/p/MLB43986681?pdp_filters=item_id%3AMLB5197656606&from=gshop&matt_tool=91562990&matt_internal_campaign_id=&matt_word=&matt_source=google&matt_campaign_id=22090193891&matt_ad_group_id=174661984004&matt_match_type=&matt_network=g&matt_device=c&matt_creative=727914181090&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=735098660&matt_product_id=MLB43986681-product&matt_product_partition_id=2392713578861&matt_target_id=aud-1967156880386:pla-2392713578861&cq_src=google_ads&cq_cmp=22090193891&cq_net=g&cq_plt=gp&cq_med=pla&gad_source=1&gad_campaignid=22090193891&gbraid=0AAAAAD93qcB9rle_WedL6atW03-Klp6f_&gclid=Cj0KCQiA-NHLBhDSARIsAIhe9X0RUuj74fUpQcMN1MXpvSVynh04RoKwYq0xg9n8QLxBREtiGrHlpbEaAl_qEALw_wcB) |
| **Resistor 160R 1/16W** | ![Resistor 160R 1/16W](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/6.res_160r.png) | [Link-6](https://curtocircuito.com.br/resistor-160r-1-4w-5.html?srsltid=AfmBOooummxSfqPPxtEGCP2oIGFGCHLwrRuSj9kSmuDmM7-aB_z1A_wM) |
| **Resistor 82R 1/16W** | ![Resistor 82R 1/16W](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/7.res_82r.png) | [Link-7](https://www.eletronicacuiaba.com/resistor-82r-1w?srsltid=AfmBOoolW35wzi-zIkhnYBgZPu3kQA6LoSaOEcVuWnducEBeAkPMe61_) |
| **Potenciometro de 10kR** | ![Potenciometro de 10kR](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/8.pot.png) | [Link-8](https://www.eletrogate.com/potenciometro-linear-10k?utm_source=Site&utm_medium=GoogleMerchant&utm_campaign=GoogleMerchant&gad_source=4&gad_campaignid=22470016551&gbraid=0AAAAADqxjs-rOciFRnyUc_60MucHtBFyV&gclid=CjwKCAiAkbbMBhB2EiwANbxtbVreQzrpeN-PZUCqvB4BPiUrgYc2YFjf_it2SdSrdO_waFcdgcZb9xoCvjkQAvD_BwE) |


<div align="justify">

### 3.1 Critérios de projeto:

<div align="justify">
A Imagem abaixo ilustra o mapa de I/Os do microcontrolador de nosso projeto:
<br>
<div align="center">

| **ESP32-S3-DEVKITC-1-N8R8** |
| :---: |
![ESP32-S3-DEVKITC-1-N8R8](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/esp_pinout.png)

<div align="justify">
A imagem nos demonstra que alguns pinos são para funções específicas do MCU, e devem ser usadas com cuidado.
Em nosso projeto, foi adotado o seguinte ma

---

## 3. Mapa de pinos e conexões
Conforme o firmware atual (`main.cpp`) e o diagrama (`diagram.json`), o mapeamento é:

| Sinal | Função | Pino (ESP32) |
|---|---|---:|
| `NTC_PIN` | Entrada analógica do NTC | GPIO **34** |
| `POT_PIN` | Entrada analógica do potenciômetro | GPIO **35** |
| `LED_PIN` | Saída para LED (resistência) | GPIO **26** |
| `BTN_PIN` | Entrada do botão (com pull-up interno) | GPIO **27** |

### Display
O display **existe no sistema**, mas pode não estar presente no diagrama da simulação.

- Se usar **LCD I2C**: normalmente `SDA`/`SCL` seguem os pinos padrão do ESP32 (ex.: **GPIO 21/22**), mas ajuste conforme sua placa.
- Se usar **LCD paralelo**: defina os GPIOs no firmware e documente aqui.

---

## 4. Comunicação MQTT e telemetria

### Broker
O firmware está configurado para um broker público (ex.: HiveMQ) e publica em tópicos fixos.

### Tópicos (exemplo do firmware)
- `talles/forno/telemetria` — JSON com as variáveis do forno
- `talles/forno/status` — mensagens simples (ex.: `LIGADO` / `DESLIGADO`)

### Payload (telemetria)
No `main.cpp`, a telemetria enviada possui este formato:

```json
{
  "temperatura": 180,
  "setpoint": 200,
  "forno": true,
  "resistencia": false
}
```

> Observação: se você migrar o controle para PWM (duty-cycle), vale publicar também `pwm` (0–100) e refletir esse valor no LED via PWM (ex.: `ledcWrite()`).

---

## 5. Arquitetura do firmware
Estrutura atual (simplificada):

- Conexão Wi-Fi
- Conexão MQTT
- Loop principal:
  - Leitura do botão (toggle liga/desliga)
  - Leitura do NTC e do potenciômetro
  - Cálculo do controle (ex.: liga/desliga com histerese, ou PWM)
  - Publicação periódica da telemetria

Arquivo principal:

- `main.cpp` — concentra conexão, leitura de sensores, controle e publicação MQTT.

---

## 6. Integração com Ubidots
Fluxo recomendado:

1. Criar um **Device** no Ubidots.
2. Criar variáveis para as chaves publicadas (ex.: `temperatura`, `setpoint`, `forno`, `resistencia` e/ou `pwm`).
3. Configurar a integração MQTT (bridge/plugin conforme o seu fluxo) para consumir do broker e alimentar o device.
4. Montar um dashboard com widgets para:
   - temperatura
   - referência (setpoint/pwm)
   - estado do forno
   - estado/nível da resistência

---

## 7. Próximos passos
- Adicionar o **display** no Wokwi e no firmware (tela com temperatura, referência e estado).
- Migrar o acionamento do LED/resistência para **PWM real** (saída proporcional).
- Calibração da conversão do NTC (em vez de `map()` linear) para aproximar o comportamento físico.
- (Opcional) Assinatura de comandos via MQTT para controle remoto (ex.: setpoint/pwm).
