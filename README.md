# Projeto prático de nº3 do programa de especialização/residência em sistemas eletrônicos embarcados.
Organizado em uma parceria MCTI/Softex + CPQD + FIAP

---

#### Para dúvidas, comentários ou sugestões fiquem à vontade, estarei à disposição através de minha página: 


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
[Wokwi](https://wokwi.com/projects/454130082230099969)

[Ubidots](https://stem.ubidots.com/app/dashboards/69701418221d689174891929)

---
## Objetivo?
<div align="justify">
<br>
•    Criar um canal no ThingSpeak com 2 a 4 campos (livre definição).

•    Configurar no Wokwi um streaming MQTT que publique mensagens (Dados de sensores, etc) no broker.

•    No ThingSpeak, usar Write API Key para configurar ingestão/consumo do tópico MQTT publicado no Broker (via integração, MQTT fields update, ou método aceito pela plataforma).

•    Verificar se os dados estão chegando no histórico do canal (gráficos e painéis).

•    Realizar testes de envio e recepção de mensagem no broker MQTT e confirmar latência/estabilidade em 1 captura de log/cliente MQTT.

•    Preencher o README simples no mesmo arquivo com orientações de funcionamento

- Criar uma documentação adequada mostrando um rascunho simples da arquitetura, objetivo do projeto, descrição geral do sistema, explicação de cada componente, seu fluxo de interação entre eles, descrição das variaveis de publicação e subscrição e conclusoes. Faça um documento formal com capa, titulo, indices, etc.

- Fazer um pequeno video de poucos minutos mostrando o funcionamento dos sistemas, pode usar captura de tela.
---
## Índice:

1. [Descrição do projeto](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#1-descri%C3%A7%C3%A3o-do-projeto)
2. [Dimensionamento e lista de materiais](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#2-dimensionamento-e-lista-de-materiais)
<br>2.1. [Cálculo do resistor para os LED](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#21-c%C3%A1lculo-do-resistor-para-os-led)
<br>2.2. [Dimensionamento dos resistores para os LEDs](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#22-dimensionamento-dos-resistores-para-os-leds)
3. [Lista de materiais utilizados nesta aplicação](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#3-lista-de-materiais-utilizados-nesta-aplica%C3%A7%C3%A3o)
<br>3.1. [Critérios de projeto](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#31-crit%C3%A9rios-de-projeto)
4. [Conexão ao canal de comunicação com o broker Mosquitto/EMQX](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#4-conex%C3%A3o-ao-canal-de-comunica%C3%A7%C3%A3o-com-o-broker-mosquittoemqx)
5. [Arquitetura do firmware](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#5-arquitetura-do-firmware)
6. [Interação ao Thingspeak](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/tree/main?tab=readme-ov-file#6-intera%C3%A7%C3%A3o-ao-thingspeak)


---
## 1. Descrição do projeto:

Este trabalho se objetivou a criação de um sistema robusto de IOT para interface de um Supermercado, permitindo que possamos ter um controle das luzes da área externa de modo intertravado, permitindo o acionamento por botões, pelo dashboard, ou ainda, de maneira autônoma pela leitura do sensor de luz.

A medição de gases tóxicos/perigosos no setor de panificação, permitindo que possamos indentificar um possível vazamento de gás antes que o mesmo se alastre, provocando uma catastrófe.

Implementação de um broker MQTT que centraliza nossos dados e ainda fornece um dashboard.

Desenvolvimento de uma IHM à partir do display LCD 320x240 - IL9341, simplificando a centralização visual de nossos dados.

### 2. Dimensionamento e lista de materiais: 


#### 2.1 Cálculo do resistor para os LED:

<div align="justify">
Serão utilizados dois leds neste contexto, o amarelo que simula uma iluminação de área externa controlado pelo relé, enquanto o led vermelho representa a luz de alerta, acionada diretamente pelo gpio, as fórmulas abaixo representam o modo de cálculo para dimensionamento:

<br>
<br>
2.1.1. Resistência mínima segura:

$$
R(\Omega) = \frac{V_{pp} - V_{led}}{I_{led}}
$$ 

<br>


2.1.2. Dissipação no resistor:

$$
P(W) = U*i
$$

2.1.3. Potência mínima do resistor:
<br>
*Fs é um fator de segurança, idealmente adoto 50%*
<br>

$$
Pmin = P(W) * (1+Fs)
$$
<br>
<br>

*Vale ressaltar que a tensão máxima de trabalho no gpio do ESP32-S3-DEVKITC-1-N8R8 é de 3.3V, portanto Vpp = 3.3V*

<div align="center">

| **Tabela de caracteristicas técnicas dos LEDs** |
| :---: |
![tabela_led](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/tabela_led.png)
| **Tabela de resistores comerciais** |
![tabela_resistores](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/tabela_resistores.png)
| **Diagrama de potências de resistores comerciais** |
![tabela_resistores](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/pot_resistores.png)

<div align="justify">

#### 2.2 Dimensionamento dos resistores para os LEDs:

O led vermelho em carga plena utiliza 1.7V, e sua corrente é de 10mA (0.01A), para tal:

<br>
<br>

$$
R(\Omega) = \frac{3.3 - 1.7}{0.01} = \frac{1.6}{0.01} = 160\Omega
$$

<br>

$$
P(W) = U*i = 1.6 * 0.01 = 0.016W
$$

<br>

$$
Pmin = U*i = 0.016W * (1+0.50)=0.024W
$$
<br>

<div align="justify">
Como vimos na tabela acima esse valor de resistor é uma medida comercial, no entanto, não existe resistor de 0.024W, o valor comercial mais próximo arredondando para cima é o de 1/16W (0.0625W).

---
O led amarelo em carga plena utiliza 1.7V, e sua corrente é de 20mA (0.02A)

<br>

$$
R(\Omega) = \frac{3.3 - 1.7}{0.02} = \frac{1.6}{0.02} = 80\Omega
$$

<br>

$$
P(W) = U*i = 1.6 * 0.02 = 0.032W
$$

<br>

$$
Pmin = U*i = 0.032W * (1+0.50)=0.048W
$$
<br>

<div align="justify">
Como vimos na tabela acima esse valor de resistor, não é uma medida comercial, portanto ao arredondar usaremos um de 82R, além disso, não existe resistor de 0.048W, o valor comercial mais próximo arredondando para cima é o de 1/16W (0.0625W).

## 3 Lista de materiais utilizados nesta aplicação:

 
<div align="center">

| **Item** | **Imagem** | **Link referêcia** |
| :--- | :---: | :---: |
| **ESP32-S3-DEVKITC-1-N8R8** | ![ESP32-S3-DEVKITC-1-N8R8](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/1.esp32.png) | [Link-1](https://www.digikey.com.br/en/products/detail/espressif-systems/ESP32-S3-DEVKITC-1-N8R8/15295894) |
| **Display IL9341** | ![Display IL9341](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/2.display.jpg) | [Link-2](https://www.mercadolivre.com.br/display-lcd-tft-ili9341--colorido-28-pol-240x320cores-vivas/up/MLBU3371688434?pdp_filters=item_id%3AMLB4166634771&from=gshop&matt_tool=48995110&matt_internal_campaign_id=&matt_word=&matt_source=google&matt_campaign_id=22603531562&matt_ad_group_id=181244933895&matt_match_type=&matt_network=g&matt_device=c&matt_creative=758138322200&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=5347036196&matt_product_id=MLBU3371688434&matt_product_partition_id=2424443211326&matt_target_id=aud-1967156880386:pla-2424443211326&cq_src=google_ads&cq_cmp=22603531562&cq_net=g&cq_plt=gp&cq_med=pla&gad_source=1&gad_campaignid=22603531562&gbraid=0AAAAAD93qcB9VwMp0SqGl6HPQYcxEhVoa&gclid=Cj0KCQiA18DMBhDeARIsABtYwT1S_ecMnkOmja57DlBwD8KJ-BQ375CuKHMsOTFzIfUaNuwHYN13974aAh4BEALw_wcB) |
| **Sensor de luminosidade (LDR)** | ![Sensor de luminosidade (LDR)](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/4.sensor_de_luminosidade.png) | [Link-4](https://www.usinainfo.com.br/sensor-de-luminosidade/modulo-sensor-de-luminosidade-ldr-hw-072-para-arduino-e-esp32-2539.html?srsltid=AfmBOoqh2a2FSQJ-WsQwSs6_H3h9eT0_MXp4nbUpsYRxL-ytK1wvdg2d) |
| **Sensor de gás** | ![Sensor de gás](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/5.sensor_de_gas.png) | [Link-5](https://www.eletrogate.com/sensor-de-gas-mq-6-glp-propano-e-iso-butano?srsltid=AfmBOor93c2GLfcym9Ea-exo15aaudNBmn3tdkNTRiq2efnVQ1H4DsF7) |
| **Módulo Relé** | ![Módulo Relé](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/6.modulo_rele.png) | [Link-6](https://www.autocorerobotica.com.br/modulo-rele-5v-1-canal-high-ou-low-trigger?srsltid=AfmBOooc7hmlZ66WW1IAvDfkgiAtKHC0fGlh_Dl073uQg76DFQskytyW) |
| **Buzzer** | ![Buzzer](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/7.modulo_buzzer.png) | [Link-7](https://38-3d.co.uk/products/active-buzzer-module-3-3v-5v?srsltid=AfmBOorTXmQ61TX4eDaEroojsn16cxy4yOKYKK0hJKutVKGQzq2nbawm) |
| **Led vermelho** | ![Led vermelho](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/8.led_vermelho.png) | [Link-8](https://www.mercadolivre.com.br/kit-1000-leds-difusos-lamp-5mm-vermelhos-para-projetos-eletrnicos/p/MLB43986681?pdp_filters=item_id%3AMLB5197656606&from=gshop&matt_tool=91562990&matt_internal_campaign_id=&matt_word=&matt_source=google&matt_campaign_id=22090193891&matt_ad_group_id=174661984004&matt_match_type=&matt_network=g&matt_device=c&matt_creative=727914181090&matt_keyword=&matt_ad_position=&matt_ad_type=pla&matt_merchant_id=735098660&matt_product_id=MLB43986681-product&matt_product_partition_id=2392713578861&matt_target_id=aud-1967156880386:pla-2392713578861&cq_src=google_ads&cq_cmp=22090193891&cq_net=g&cq_plt=gp&cq_med=pla&gad_source=1&gad_campaignid=22090193891&gbraid=0AAAAAD93qcB9rle_WedL6atW03-Klp6f_&gclid=Cj0KCQiA-NHLBhDSARIsAIhe9X0RUuj74fUpQcMN1MXpvSVynh04RoKwYq0xg9n8QLxBREtiGrHlpbEaAl_qEALw_wcB) |
| **Led amarelo** | ![Led amarelo](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/9.led_amarelo.png) | [Link-9](https://www.dualshop.com.br/led-5mm-amarelo-amarelo-standard-difuso-pth?srsltid=AfmBOop2r85YTH3l7Al_zvDkvm4G4wAGqd8xIjWce4ekCzflslonKj03) |
| **Resistor 160R 1/16W** | ![Resistor 160R 1/16W](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/10.res_160r.png) | [Link-10](https://curtocircuito.com.br/resistor-160r-1-4w-5.html?srsltid=AfmBOooummxSfqPPxtEGCP2oIGFGCHLwrRuSj9kSmuDmM7-aB_z1A_wM) |
| **Resistor 82R 1/16W** | ![Resistor 82R 1/16W](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/11.res_82r.png) | [Link-11](https://www.eletronicacuiaba.com/resistor-82r-1w?srsltid=AfmBOoolW35wzi-zIkhnYBgZPu3kQA6LoSaOEcVuWnducEBeAkPMe61_) |
| **Push-Button** | ![Push-Button](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/10.push_button.png) | [Link-12](https://www.eletrogate.com/push-button-chave-tactil-6x6x6mm?srsltid=AfmBOop5mCOiH2MCIOyZxYxCbOE5eBJbDxK-dSsRCbRR-rpGESQx8RGx) |

<div align="justify">

### 3.1 Critérios de projeto:

<div align="justify">
A Imagem abaixo ilustra o mapa de I/Os do microcontrolador de nosso projeto:
<br>
<div align="center">

<br>


| **ESP32-S3-DEVKITC-1-N8R8** |
| :---: |
![ESP32-S3-DEVKITC-1-N8R8](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/esp_pinout.png)

<div align="justify">
A imagem nos demonstra que alguns pinos são para funções específicas do MCU, e devem ser usadas com cuidado.
Em nosso projeto, foi adotado o seguinte mapa de conexões:

<br>

<div align="center">

<br>

| **Pino** | **Função** |
| :---: | :--- |
| GPIO 11 | Entrada Digital: Sensor de Gás 1 (MQ — saída digital, ativo em LOW) |
| GPIO 12 | Entrada Digital: Sensor de Gás 2 (MQ — saída digital, ativo em LOW) |
| GPIO 13 | Entrada Digital: Sensor de Gás 3 (MQ — saída digital, ativo em LOW) |
| GPIO 14 | Entrada Digital: Sensor de Gás 4 (MQ — saída digital, ativo em LOW) |
| GPIO 9 | Entrada Digital: Sensor de Luminosidade (LDR) 1 |
| GPIO 46 | Entrada Digital: Sensor de Luminosidade (LDR) 2 |
| GPIO 3 | Entrada Digital: Sensor de Luminosidade (LDR) 3 |
| GPIO 10 | Entrada Digital: Sensor de Luminosidade (LDR) 4 |
| GPIO 16 | Saída Digital: Relé 1 |
| GPIO 17 | Saída Digital: Relé 2 |
| GPIO 18 | Saída Digital: Relé 3 |
| GPIO 8 | Saída Digital: Relé 4 |
| GPIO 7 | Entrada Digital: Botão 1 (latch) |
| GPIO 6 | Entrada Digital: Botão 2 (latch) |
| GPIO 5 | Entrada Digital: Botão 3 (latch) |
| GPIO 4 | Entrada Digital: Botão 4 (latch) |
| GPIO 2 | Saída Digital: Sirene/Alarme de Gás |
| GPIO 42 | Saída Digital (SPI): TFT - CS |
| GPIO 40 | Saída Digital (SPI): TFT - DC |
| GPIO 41 | Saída Digital (SPI): TFT - RST |
| GPIO 38 | Saída Digital (SPI): TFT - SCK |
| GPIO 39 | Saída Digital (SPI): TFT - MOSI |
| GPIO 36 | Entrada Digital (SPI): TFT - MISO |

<div align="justify">

Essa é a representação visual das conexões:

<div align="center">

| **Diagrama de conexões** |
| :---: |
![Diagrama de conexões](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/ope_tel_0.png)

<div align="justify">

Essa é a representação visual das conexões:

<div align="center">

| **Ação** |
| :--- |
| O 1º estágio do boot transiciona para uma breve tela exibindo "Display inicializado"|
|![Ope_tel_1](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/ope_tel_1.png)|
| O 2º estágio do boot apresenta rapidamente o escopo de projeto e exibe o nome do projetista.|
|![Ope_tel_2](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/ope_tel_2.png)|
| O 3º estágio do boot demonstra os organizadores do curso.|
|![Ope_tel_3](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/ope_tel_3.png)|
| A tela final demonstra o processo de operação em regime permanente.|
|![Ope_tel_4](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/ope_tel_4.png)|


<div align="justify">

## 5. Arquitetura do firmware:

<div align="center">

| **Arquivo** | **Função** |
| :--- | :--- |
| [main.cpp](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/main.cpp)   | Responsável por processar e executar as funções primárias. |
| [wifi.cpp](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/wifi.cpp)   | Centraliza os comandos necessários para conexão ao wi-fi. |
| [http.cpp](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/http.cpp)  | Centraliza os comandos necessários para conexão ao broker. |
| [tft.cpp](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/tft.cpp) | Adiciona o suporte ao uso do display TFT LCD 320x240 IL9341 |
| [img1.cpp](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/img1.cpp) | Adiciona o cabeçalho de inicialização 1, que pode ser visto na próxima secção.|
| [img2.cpp](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/img1.cpp) | Adiciona o cabeçalho de inicialização 2, que pode ser visto na próxima secção.|
| [tft.h](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/wifi_con.h) | Chamada das funções recursivas de conexão ao display. |
| [http.h](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/src/mqtt_con.h) | Chamada das funções recursivas de conexão ao HTTP. |

<div align="justify">

## 6. Interação ao Thingspeak:

Através desse processo torna-se possível a implementação de um dashboard para montiramento das grandezas:

| **Imagens do dashboard em operação** |
| :--- |
|![dash_1](https://github.com/IOT-Eletric-Oven-FIAP/fiap_embedded_project_iot_eletric_oven/blob/main/images/dash_1.png)|

---

