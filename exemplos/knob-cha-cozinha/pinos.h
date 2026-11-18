// pinos.h — gerado pelo nó a nó a partir dos nós do projeto "Knob: chá e cozinha — ESP32-S3 + TFT + joystick"
// 18/11/2026. Confira com o seu circuito antes de usar.
#pragma once

#define PINO_SCK           15   // fio amarelo: ESP32-S3 GPIO15 -> TFT SCL
#define PINO_MOSI          16   // fio verde: ESP32-S3 GPIO16 -> TFT SDA
#define PINO_RST           17   // fio laranja: ESP32-S3 GPIO17 -> TFT RES
#define PINO_DC            18   // fio roxo: ESP32-S3 GPIO18 -> TFT DC
#define PINO_CS             8   // fio azul: ESP32-S3 GPIO8 -> TFT CS
#define PINO_BL             9   // fio marrom: ESP32-S3 GPIO9 -> TFT BL
#define PINO_BTN_A         12   // fio branco: ESP32-S3 GPIO12 -> BTN-A A
#define PINO_BTN_B         13   // fio rosa: ESP32-S3 GPIO13 -> BTN-B A
#define PINO_VRX            4   // fio ciano: ESP32-S3 GPIO4 -> Joystick VRx
#define PINO_VRY            5   // fio cinza: ESP32-S3 GPIO5 -> Joystick VRy
#define PINO_SW             6   // fio roxo: ESP32-S3 GPIO6 -> Joystick SW

// Alimentação e outros nós (sem #define):
//   3V3      (energia, fio vermelho): ESP32-S3 3V3 -> TFT VCC -> Joystick +5V
//   GND      (terra, fio preto): ESP32-S3 GND -> TFT GND -> BTN-A B -> BTN-B B -> Joystick GND
