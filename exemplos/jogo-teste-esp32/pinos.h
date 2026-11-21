// pinos.h — gerado pelo nó a nó a partir dos nós do projeto "Jogo Pelourinho — ESP32 (WROOM-32) + TFT + 2 botões"
// 21/11/2026. Confira com o seu circuito antes de usar.
#pragma once

#define PINO_SCK            4   // fio amarelo: ESP32 GPIO4 -> TFT SCL
#define PINO_MOSI          16   // fio verde: ESP32 GPIO16 -> TFT SDA
#define PINO_RST           17   // fio laranja: ESP32 GPIO17 -> TFT RES
#define PINO_DC            18   // fio roxo: ESP32 GPIO18 -> TFT DC
#define PINO_CS            19   // fio azul: ESP32 GPIO19 -> TFT CS
#define PINO_BL            21   // fio marrom: ESP32 GPIO21 -> TFT BL
#define PINO_BTN_A         14   // fio branco: ESP32 GPIO14 -> BTN-A A
#define PINO_BTN_B         27   // fio rosa: ESP32 GPIO27 -> BTN-B A

// Alimentação e outros nós (sem #define):
//   3V3      (energia, fio vermelho): ESP32 3V3 -> TFT VCC
//   GND      (terra, fio preto): ESP32 GND -> TFT GND -> BTN-A B -> BTN-B B
