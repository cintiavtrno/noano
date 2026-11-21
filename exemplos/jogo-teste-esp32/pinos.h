// pinos.h — gerado pelo nó a nó a partir dos nós do projeto "Jogo Pelourinho — ESP32 (WROOM-32) + TFT + 2 botões"
// 21/11/2026. Confira com o seu circuito antes de usar.
#pragma once

#define PINO_SCK           18   // fio amarelo: ESP32 GPIO18 -> TFT SCL
#define PINO_MOSI          23   // fio verde: ESP32 GPIO23 -> TFT SDA
#define PINO_RST           22   // fio laranja: ESP32 GPIO22 -> TFT RES
#define PINO_DC            21   // fio roxo: ESP32 GPIO21 -> TFT DC
#define PINO_CS             5   // fio azul: ESP32 GPIO5 -> TFT CS
#define PINO_BTN_A         32   // fio branco: ESP32 GPIO32 -> BTN-A A
#define PINO_BTN_B         33   // fio rosa: ESP32 GPIO33 -> BTN-B A

// Alimentação e outros nós (sem #define):
//   3V3      (energia, fio vermelho): ESP32 3V3 -> TFT VCC -> TFT BL
//   GND      (terra, fio preto): ESP32 GND -> TFT GND -> BTN-A B -> BTN-B B
