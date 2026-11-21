// Teste da montagem — nó a nó
// ESP32-WROOM-32 (DevKit V1, 30 pinos) + TFT 1,8" ST7735 + 2 botões 6×6
// Mesmos pinos do DoisJogos (BL ligado direto no 3V3).
// Rode ANTES de soldar e de novo DEPOIS de soldar: se as 3 faixas de cor
// aparecerem e os dois botões acenderem na tela, a montagem está certa.
// Placa na Arduino IDE: "ESP32 Dev Module".
// Bibliotecas (Gerenciador de Bibliotecas da Arduino IDE):
//   "Adafruit ST7735 and ST7789 Library" e "Adafruit GFX Library"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "pinos.h"   // pinos gerados pelo nó a nó a partir dos nós

Adafruit_ST7735 tft = Adafruit_ST7735(&SPI, PINO_CS, PINO_DC, PINO_RST);

const int BOTOES[2] = {PINO_BTN_A, PINO_BTN_B};
const char *NOMES[2] = {"A", "B"};
bool antes[2] = {false, false};

void desenhaBotao(int i, bool apertado) {
  int x = 20 + i * 76;
  tft.fillRoundRect(x, 80, 44, 36, 6, apertado ? ST77XX_YELLOW : 0x2945);
  tft.setTextColor(apertado ? ST77XX_BLACK : ST77XX_WHITE);
  tft.setCursor(x + 19, 94);
  tft.print(NOMES[i]);
}

void setup() {
  Serial.begin(115200);
  // Sem resistor: o ESP liga um resistor interno (pull-up) e o botão leva o pino ao GND.
  for (int i = 0; i < 2; i++) pinMode(BOTOES[i], INPUT_PULLUP);


  SPI.begin(PINO_SCK, -1, PINO_MOSI, PINO_CS);
  tft.initR(INITR_BLACKTAB);  // se a borda ou as cores saírem erradas: INITR_GREENTAB ou INITR_REDTAB
  tft.setRotation(1);         // tela deitada (160 × 128)
  tft.fillScreen(ST77XX_BLACK);

  // Faixas na ordem vermelho, verde, azul = SPI e tela ok
  tft.fillRect(0, 0, 53, 40, ST77XX_RED);
  tft.fillRect(53, 0, 54, 40, ST77XX_GREEN);
  tft.fillRect(107, 0, 53, 40, ST77XX_BLUE);

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(4, 48);
  tft.print("teste da montagem");
  tft.setCursor(4, 60);
  tft.print("aperte cada botao");
  for (int i = 0; i < 2; i++) desenhaBotao(i, false);
}

void loop() {
  for (int i = 0; i < 2; i++) {
    bool apertado = digitalRead(BOTOES[i]) == LOW;
    if (apertado != antes[i]) {
      antes[i] = apertado;
      desenhaBotao(i, apertado);
      if (apertado) {
        Serial.print("apertou ");
        Serial.println(NOMES[i]);
      }
    }
  }
  delay(10);
}
