// pinos.h — placa 5×10 do jogo, com a tela encaixada direto (nó a nó, 21/11/2026)
#pragma once

#define PINO_SCK           18   // ESP32 GPIO18 -> TFT SCK
#define PINO_MOSI          23   // ESP32 GPIO23 -> TFT SDA
#define PINO_RST            5   // ESP32 GPIO5  -> TFT RESET
#define PINO_DC            21   // ESP32 GPIO21 -> TFT A0
#define PINO_CS             4   // ESP32 GPIO4  -> TFT CS
#define PINO_BTN_A         16   // ESP32 GPIO16 (RX2) -> botão A
#define PINO_BTN_B         15   // ESP32 GPIO15 (D15) -> botão B

// Alimentação:
//   3V3: ESP32 3V3 -> TFT VCC -> TFT LED
//   GND: ESP32 GND -> TFT GND -> botão A -> botão B
