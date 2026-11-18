// knob · chá e cozinha — nó a nó
// ESP32-S3 + TFT 1,8" ST7735 (160×128) + joystick (como knob) + 2 botões
// Cenas desenhadas por código, inspiradas nas animações de knob em AMOLED.
//
// Como usar:
//   - Gire a alavanca do joystick em círculo: horário avança a cena, anti-horário volta.
//   - Segurar a alavanca para a direita/esquerda também anda, devagar.
//   - Clique do joystick: volta a cena ao começo.  Botão A: próxima cena.  Botão B: anterior.
//
// Bibliotecas (Gerenciador de Bibliotecas da Arduino IDE):
//   "Adafruit ST7735 and ST7789 Library" e "Adafruit GFX Library"
// Placa na Arduino IDE: "ESP32S3 Dev Module".
// Se o knob girar ao contrário, troque SENTIDO para -1.

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "pinos.h"

#define SENTIDO 1          // 1 ou -1
#define VOLTA 0.40f        // uma volta completa anda 40% da cena

const int W = 160, H = 128;
Adafruit_ST7735 tft = Adafruit_ST7735(&SPI, PINO_CS, PINO_DC, PINO_RST);
GFXcanvas16 cv(W, H);      // a cena é desenhada aqui e vai inteira para a tela (sem piscar)

// ---------- cores ----------
constexpr uint16_t C565(uint32_t h) { return ((h >> 8) & 0xF800) | ((h >> 5) & 0x07E0) | ((h >> 3) & 0x001F); }
const uint16_t PRETO = C565(0x000000), BRANCO = C565(0xF2F2EE), CINZA = C565(0x6E787D),
  LIMA = C565(0xD8F05A), LIMA_FUN = C565(0x2C3A08),
  PEP_CASCA = C565(0x2F7D3A), PEP_POLPA = C565(0xC8EC8A), PEP_SEM = C565(0x8FBF55),
  LARANJA = C565(0xFF8A1F), LARANJA_ESC = C565(0xB85A0C), FOLHA = C565(0x5FBF4A),
  BAT_CASCA = C565(0xB07A3E), BAT_PINTA = C565(0x7A5024), BAT_POLPA = C565(0xF4E2A0),
  LAMINA = C565(0x9AA4A9), CABO = C565(0x3A2A20), MADEIRA = C565(0x5A4632);

uint16_t mix(uint16_t a, uint16_t b, float t) {
  int ar = (a >> 11) & 31, ag = (a >> 5) & 63, ab = a & 31, br = (b >> 11) & 31, bg = (b >> 5) & 63, bb = b & 31;
  return (uint16_t)(((int)(ar + (br - ar) * t) << 11) | ((int)(ag + (bg - ag) * t) << 5) | (int)(ab + (bb - ab) * t));
}

// ---------- utilidades ----------
struct V { float x, y; };
float clampf(float v, float a, float b) { return v < a ? a : v > b ? b : v; }
float lerpf(float a, float b, float t) { return a + (b - a) * t; }
float ss(float a, float b, float x) { float t = clampf((x - a) / (b - a), 0, 1); return t * t * (3 - 2 * t); }
float rnd(int i) { float x = sinf(i * 127.1f + 311.7f) * 43758.5453f; return x - floorf(x); }

void px(float x, float y, uint16_t c) { cv.drawPixel((int)x, (int)y, c); }
void hline(int x0, int x1, int y, uint16_t c) { if (x1 >= x0) cv.drawFastHLine(x0, y, x1 - x0 + 1, c); }
void line(float x0, float y0, float x1, float y1, uint16_t c) { cv.drawLine(lroundf(x0), lroundf(y0), lroundf(x1), lroundf(y1), c); }
void thick(float x0, float y0, float x1, float y1, uint16_t c) {
  line(x0, y0, x1, y1, c);
  if (fabsf(x1 - x0) > fabsf(y1 - y0)) line(x0, y0 + 1, x1, y1 + 1, c); else line(x0 + 1, y0, x1 + 1, y1, c);
}
// polígono preenchido por linhas (mesmo algoritmo da prévia no navegador)
void fillPoly(const V *p, int n, uint16_t c) {
  float y0 = 1e9, y1 = -1e9;
  for (int i = 0; i < n; i++) { y0 = min(y0, p[i].y); y1 = max(y1, p[i].y); }
  float xs[48];
  for (int y = max(0, (int)ceilf(y0 - .5f)); y <= min(H - 1, (int)floorf(y1 - .5f)); y++) {
    float yy = y + .5f; int k = 0;
    for (int i = 0; i < n && k < 48; i++) {
      V a = p[i], b = p[(i + 1) % n];
      if ((a.y <= yy && b.y > yy) || (b.y <= yy && a.y > yy)) xs[k++] = a.x + (yy - a.y) * (b.x - a.x) / (b.y - a.y);
    }
    for (int i = 1; i < k; i++) { float v = xs[i]; int j = i - 1; while (j >= 0 && xs[j] > v) { xs[j + 1] = xs[j]; j--; } xs[j + 1] = v; }
    for (int i = 0; i + 1 < k; i += 2) hline(lroundf(xs[i]), lroundf(xs[i + 1]) - 1, y, c);
  }
}
void poly(const V *p, int n, uint16_t c, bool fechado = true) {
  for (int i = 0; i < n - (fechado ? 0 : 1); i++) { V a = p[i], b = p[(i + 1) % n]; line(a.x, a.y, b.x, b.y, c); }
}
// pontos de uma elipse (ou arco); devolve quantos pontos escreveu
int ell(V *o, float cx, float cy, float rx, float ry, int n, float a0 = 0, float a1 = TWO_PI) {
  bool cheio = (a1 - a0) >= TWO_PI - 1e-4f; int m = cheio ? n : n + 1;
  for (int i = 0; i < m; i++) { float a = a0 + (a1 - a0) * i / n; o[i] = { cx + cosf(a) * rx, cy + sinf(a) * ry }; }
  return m;
}
// gira em volta da origem e move
void xf(V *p, int n, float ang, float tx, float ty) {
  float c = cosf(ang), s = sinf(ang);
  for (int i = 0; i < n; i++) { float x = p[i].x, y = p[i].y; p[i] = { x * c - y * s + tx, x * s + y * c + ty }; }
}
V xp(float x, float y, float ang, float tx, float ty) { V v = { x, y }; xf(&v, 1, ang, tx, ty); return v; }

// ---------- cena 1: chá ----------
void cenaCha(float p, float t) {
  V a[40], b[40]; int n;
  float f = ss(.38f, .82f, p);
  float tilt = -(ss(.12f, .36f, p) - ss(.84f, .98f, p)) * 1.0f;
  float go = ss(.08f, .34f, p) - ss(.86f, .99f, p);
  n = ell(a, 48, 111, 20, 3, 20); poly(a, n, CINZA);
  V cup[4] = { {37, 90}, {59, 90}, {56, 108}, {40, 108} };
  if (f > 0) {
    float ly = lerpf(107, 92, f), k0 = (ly - 90) / 18;
    V liq[4] = { {lerpf(37, 40, k0), ly}, {lerpf(59, 56, k0), ly}, {56, 108}, {40, 108} };
    fillPoly(liq, 4, LIMA); hline(lroundf(lerpf(37, 40, k0)), lroundf(lerpf(59, 56, k0)), lroundf(ly), BRANCO);
  }
  poly(cup, 4, BRANCO); n = ell(a, 48, 90, 11, 2, 16); poly(a, n, BRANCO);
  n = ell(a, 62, 98, 4, 5, 10, -HALF_PI, HALF_PI); poly(a, n, BRANCO, false);
  if (f > .55f) {
    float am = ss(.55f, .85f, f);
    for (int sx : { 44, 52 }) {
      bool tem = false; float px0 = 0, py0 = 0;
      for (float y = 86; y >= 86 - 18 * am; y -= 2) { float x = sx + sinf(y * .35f + t * 3 + sx) * 2; if (tem) line(px0, py0, x, y, CINZA); px0 = x; py0 = y; tem = true; }
    }
  }
  float bx = lerpf(114, 86, go), by = lerpf(80, 56, go);
  n = ell(a, 0, 0, 17, 12, 28); xf(a, n, tilt, bx, by);
  float resto = 1 - f * .85f; fillPoly(a, n, mix(LIMA_FUN, LIMA, resto));
  int m = ell(b, -3, 3, 10, 6, 16); xf(b, m, tilt, bx, by); fillPoly(b, m, mix(LIMA_FUN, LIMA, min(1.0f, resto + .2f)));
  poly(a, n, BRANCO);
  V s1[3] = { {-15, 2}, {-24, -3}, {-30, -10} }, s2[3] = { {-14, 7}, {-26, 0}, {-31, -8} };
  xf(s1, 3, tilt, bx, by); xf(s2, 3, tilt, bx, by); poly(s1, 3, BRANCO, false); poly(s2, 3, BRANCO, false);
  n = ell(a, 18, 0, 6, 8, 12, -HALF_PI, HALF_PI); xf(a, n, tilt, bx, by); poly(a, n, BRANCO, false);
  n = ell(a, 0, -12, 8, 2.5f, 14); xf(a, n, tilt, bx, by); poly(a, n, BRANCO);
  n = ell(a, 0, -16, 2.2f, 2.2f, 8); xf(a, n, tilt, bx, by); poly(a, n, BRANCO);
  float s = ss(.3f, .4f, p) * (1 - ss(.8f, .88f, p));
  if (s > .05f) {
    V T = xp(-30.5f, -9, tilt, bx, by); float ex = 48, ey = lerpf(92, 105, 1 - f); float qx = T.x, qy = T.y;
    for (int i = 1; i <= 14; i++) {
      float u = i / 14.0f; float x = lerpf(T.x, ex, u) + sinf(u * PI) * -4, y = lerpf(T.y, ey, u * u);
      if (s > .5f) thick(qx, qy, x, y, LIMA); else line(qx, qy, x, y, LIMA);
      qx = x; qy = y;
    }
  }
}

// ---------- cena 2: pepino ----------
void cenaPepino(float p, float t) {
  V a[40]; int n;
  const int N = 12; float k = p * N; int done = min(N, (int)floorf(k)); float fr = k - done;
  V tab[4] = { {12, 103}, {148, 103}, {152, 112}, {8, 112} };
  fillPoly(tab, 4, MADEIRA); poly(tab, 4, BRANCO); line(8, 112, 8, 115, BRANCO); line(152, 112, 152, 115, BRANCO); line(8, 115, 152, 115, BRANCO);
  float x0 = 22, len = 96, sw = len / N, xEnd = x0 + len - done * sw;
  if (done < N) {
    n = ell(a, x0 + 6, 93, 6, 8, 10, HALF_PI, PI * 1.5f); a[n++] = { xEnd, 85 }; a[n++] = { xEnd, 101 };
    fillPoly(a, n, PEP_CASCA); poly(a, n, BRANCO);
    n = ell(a, xEnd, 93, 2.5f, 8, 14); fillPoly(a, n, PEP_POLPA); poly(a, n, BRANCO);
    for (int i = 0; i < 5; i++) px(xEnd, 89 + i * 2, PEP_SEM);
    for (int i = 0; i < 6; i++) { float xx = x0 + 10 + i * 14; if (xx < xEnd - 3) line(xx, 88, xx + 5, 88, PEP_POLPA); }
  }
  for (int i = 0; i < done; i++) {
    float sx = 124 + (i % 4) * 6 + ((i / 4) % 2) * 3, sy = 99 - (i / 4) * 4;
    n = ell(a, sx, sy, 2.5f, 6, 12); fillPoly(a, n, PEP_POLPA); poly(a, n, PEP_CASCA); px(sx, sy, PEP_SEM);
  }
  if (done < N) {
    float xc = xEnd - sw, dn = fr < .6f ? ss(0, .6f, fr) : 1 - ss(.6f, 1, fr), ye = lerpf(70, 100, dn), ang = -.12f;
    V bl[4] = { {-14, 0}, {34, 0}, {34, -13}, {-8, -12} }; xf(bl, 4, ang, xc, ye); fillPoly(bl, 4, LAMINA); poly(bl, 4, BRANCO);
    V cb[4] = { {34, -3}, {60, -3}, {62, -11}, {34, -11} }; xf(cb, 4, ang, xc, ye); fillPoly(cb, 4, CABO); poly(cb, 4, BRANCO);
    V r1 = xp(42, -7, ang, xc, ye), r2 = xp(52, -7, ang, xc, ye); px(r1.x, r1.y, BRANCO); px(r2.x, r2.y, BRANCO);
  }
}

// ---------- cena 3: batata ----------
void cenaBatata(float p, float t) {
  const float cx = 80, cy = 60, rx = 26, ry = 31; const int N = 8;
  float k = p * N; int done = min(N, (int)floorf(k)); float fr = k - done;
  V sh[36];
  for (int i = 0; i < 36; i++) { float a = i / 36.0f * TWO_PI; float R = 1 + .05f * sinf(a * 3 + 1) + .03f * sinf(a * 5); sh[i] = { cx + cosf(a) * rx * R, cy + sinf(a) * ry * R }; }
  fillPoly(sh, 36, BAT_CASCA);
  for (int i = 0; i < 14; i++) { float a = rnd(i) * TWO_PI, r = sqrtf(rnd(i + 50)) * .8f; float x = cx + cosf(a) * rx * r, y = cy + sinf(a) * ry * r; px(x, y, BAT_PINTA); px(x + 1, y, BAT_PINTA); }
  float left = cx - rx * 1.08f, stripe = (rx * 2.16f) / N, pyl = lerpf(cy - ry, cy + ry, fr);
  for (int y = (int)floorf(cy - ry * 1.1f); y <= cy + ry * 1.1f; y++) {
    float lim = left + done * stripe + ((y < pyl && done < N) ? stripe : 0);
    float yy = y + .5f, xa = 1e9, xb = -1e9; int cnt = 0;
    for (int i = 0; i < 36; i++) { V a = sh[i], b = sh[(i + 1) % 36]; if ((a.y <= yy && b.y > yy) || (b.y <= yy && a.y > yy)) { float x = a.x + (yy - a.y) * (b.x - a.x) / (b.y - a.y); xa = min(xa, x); xb = max(xb, x); cnt++; } }
    if (cnt >= 2) { int ia = lroundf(xa), ib = min((int)lroundf(xb) - 1, (int)lroundf(lim)); hline(ia, ib, y, BAT_POLPA); }
  }
  poly(sh, 36, BRANCO);
  for (int i = 0; i < done * 2; i++) { float x = 40 + rnd(i + 9) * 80, y = 110 + rnd(i + 20) * 12; line(x, y, x + 3, y - 1, BAT_CASCA); line(x + 3, y - 1, x + 5, y + 1, BAT_CASCA); }
  if (done < N) {
    float x = left + done * stripe + stripe * .5f, y = pyl, ang = -.25f;
    V lam[4] = { {-7, -3}, {7, -3}, {7, 0}, {-7, 0} }; xf(lam, 4, ang, x, y); fillPoly(lam, 4, LAMINA); poly(lam, 4, BRANCO);
    V gar[5] = { {-7, -1}, {-9, 10}, {0, 14}, {9, 10}, {7, -1} }; xf(gar, 5, ang, x, y); poly(gar, 5, BRANCO, false);
    V cb[4] = { {-2, 14}, {2, 14}, {3, 38}, {-3, 38} }; xf(cb, 4, ang, x, y); fillPoly(cb, 4, CABO); poly(cb, 4, BRANCO);
  }
}

// ---------- cena 4: cenoura ----------
void cenaCenoura(float p, float t) {
  const float ang = -.42f, gx = 74, gy = 58;
  V r1[4] = { {-22, -46}, {22, -46}, {22, 46}, {-22, 46} }; xf(r1, 4, ang, gx, gy); poly(r1, 4, BRANCO);
  V r2[4] = { {-18, -42}, {18, -42}, {18, 42}, {-18, 42} }; xf(r2, 4, ang, gx, gy); poly(r2, 4, CINZA);
  for (int r = 0; r < 7; r++) for (int c = 0; c < 3; c++) { V s[2] = { {-12.0f + c * 12, -34.0f + r * 11}, {-6.0f + c * 12, -34.0f + r * 11} }; xf(s, 2, ang, gx, gy); line(s[0].x, s[0].y, s[1].x, s[1].y, BRANCO); }
  V cb[4] = { {-6, -46}, {6, -46}, {6, -56}, {-6, -56} }; xf(cb, 4, ang, gx, gy); poly(cb, 4, BRANCO);
  int M = (int)floorf(p * 90);
  for (int i = 0; i < M; i++) { float u = rnd(i + 3) * 2 - 1, x = 86 + u * 26, h = (1 - fabsf(u)) * 14 * rnd(i + 7), y = 124 - h, a = rnd(i + 11) * 3; line(x, y, x + cosf(a) * 3, y + sinf(a) * 2, (i % 3) ? LARANJA : LARANJA_ESC); }
  float ph = fmodf(p * 7, 1.0f), osc = 1 - fabsf(2 * ph - 1), along = lerpf(-4, 32, osc), L = max(8.0f, 46 * (1 - .8f * p));
  V base = xp(0, along, ang, gx, gy); float ca = ang - 1.25f;
  V cc[5] = { {-2, -5}, {L, -3.5f}, {L + 2, 0}, {L, 3.5f}, {-2, 5} }; xf(cc, 5, ca, base.x, base.y); fillPoly(cc, 5, LARANJA); poly(cc, 5, BRANCO);
  for (int i = 1; i < 4; i++) { V q[2] = { {i * L / 4, -3}, {i * L / 4, -1} }; xf(q, 2, ca, base.x, base.y); line(q[0].x, q[0].y, q[1].x, q[1].y, LARANJA_ESC); }
  for (int i = -1; i <= 1; i++) { V q[2] = { {L + 2, 0}, {L + 10, i * 4.0f} }; xf(q, 2, ca, base.x, base.y); line(q[0].x, q[0].y, q[1].x, q[1].y, FOLHA); }
}

typedef void (*Cena)(float, float);
Cena CENAS[] = { cenaCha, cenaPepino, cenaBatata, cenaCenoura };
const int NCENAS = 4;

// ---------- entrada: joystick como knob ----------
int cx0 = 2048, cy0 = 2048;       // centro do joystick (calibrado no início)
float alvo = 0, mostrado = 0, angAnt = 0; bool temAng = false; unsigned long paradoDesde = 0;
int cena = 0; bool antA = false, antB = false, antSW = false;

void calibra() { long sx = 0, sy = 0; for (int i = 0; i < 32; i++) { sx += analogRead(PINO_VRX); sy += analogRead(PINO_VRY); delay(2); } cx0 = sx / 32; cy0 = sy / 32; }

void leKnob() {
  float dx = (analogRead(PINO_VRX) - cx0) / 2048.0f, dy = (analogRead(PINO_VRY) - cy0) / 2048.0f;
  float mag = sqrtf(dx * dx + dy * dy);
  if (mag > .55f) {
    float a = atan2f(dy, dx);
    if (temAng) {
      float d = a - angAnt; if (d > PI) d -= TWO_PI; if (d < -PI) d += TWO_PI;
      alvo = clampf(alvo + SENTIDO * d / TWO_PI * VOLTA, 0, 1);
      if (fabsf(d) > .02f) paradoDesde = millis();
      else if (millis() - paradoDesde > 350 && fabsf(dx) > .8f) alvo = clampf(alvo + (dx > 0 ? 1 : -1) * SENTIDO * .006f, 0, 1);  // segurar de lado
    } else paradoDesde = millis();
    angAnt = a; temAng = true;
  } else temAng = false;
}
void trocaCena(int d) { cena = (cena + d + NCENAS) % NCENAS; alvo = 0; mostrado = 0; }

void setup() {
  Serial.begin(115200);
  pinMode(PINO_BTN_A, INPUT_PULLUP); pinMode(PINO_BTN_B, INPUT_PULLUP); pinMode(PINO_SW, INPUT_PULLUP);
  pinMode(PINO_BL, OUTPUT); digitalWrite(PINO_BL, HIGH);
  analogReadResolution(12);
  SPI.begin(PINO_SCK, -1, PINO_MOSI, PINO_CS);
  tft.initR(INITR_BLACKTAB);      // se a borda ou as cores saírem erradas: INITR_GREENTAB ou INITR_REDTAB
  tft.setSPISpeed(40000000);
  tft.setRotation(1);             // deitada: 160 × 128
  tft.fillScreen(ST77XX_BLACK);
  calibra();                      // não mexa no joystick nos primeiros instantes
}

void loop() {
  leKnob();
  bool a = digitalRead(PINO_BTN_A) == LOW, b = digitalRead(PINO_BTN_B) == LOW, sw = digitalRead(PINO_SW) == LOW;
  if (a && !antA) trocaCena(1);
  if (b && !antB) trocaCena(-1);
  if (sw && !antSW) alvo = 0;
  antA = a; antB = b; antSW = sw;

  mostrado += (alvo - mostrado) * .25f;
  cv.fillScreen(PRETO);
  CENAS[cena](mostrado, millis() / 1000.0f);
  cv.drawFastHLine(0, H - 1, (int)(mostrado * W), C565(0xE8A066));   // barrinha de progresso
  tft.drawRGBBitmap(0, 0, cv.getBuffer(), W, H);
}
