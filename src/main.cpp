#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Определение пинов
#define TFT_SCL   9   // SPI Clock
#define TFT_SDA   10  // SPI MOSI (данные от MCU к дисплею)
#define TFT_RS    8   // DC/Data Command
#define TFT_CS    11  // Chip Select
#define TFT_RES   18  // Reset
#define TFT_BLK   16  // Backlight control

#define TFT_H 80  // 76
#define TFT_W 284 // 284

SPIClass hspi(HSPI);
Adafruit_ST7735 tft = Adafruit_ST7735(&hspi, TFT_CS, TFT_RS, TFT_RES);

const uint16_t backgroundColors[] = {
    ST77XX_BLUE,
    ST77XX_RED,
    ST77XX_GREEN,
    ST77XX_ORANGE,
    ST77XX_CYAN,
    ST77XX_MAGENTA,
    ST77XX_YELLOW};
const int numColors = sizeof(backgroundColors) / sizeof(backgroundColors[0]);

void generateRandomBackground()
{
  // Быстрая заливка случайным цветом
  tft.fillScreen(backgroundColors[random(numColors)]);

  // Добавляем быстрые графические элементы для разнообразия
  for (int i = 0; i < 20; i++)
  {
    int x = random(tft.width());
    int y = random(tft.height());
    int size = random(5, 20);
    uint16_t color = backgroundColors[random(numColors)];

    // Рисуем простые фигуры для скорости
    switch (random(3))
    {
    case 0:
      tft.fillRect(x, y, size, size, color);
      break;
    case 1:
      tft.fillCircle(x, y, size / 2, color);
      break;
    case 2:
      tft.drawFastHLine(x, y, size, color);
      break;
    }
  }
}

void setup() {
  hspi.begin(TFT_SCL, -1, TFT_SDA, TFT_CS);
  Serial.begin(115200);
  delay(1000);
  
  // Инициализация подсветки
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH); // Включить подсветку
  tft.initR(INITR_MINI160x80_PLUGIN);    // Для красной платы

  tft.setRotation(3);  // Ориентация: 0-3
  tft.fillScreen(ST7735_BLACK);
  tft.invertDisplay(false);

  // Вывод тестового текста
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10, 0);
  tft.println("Hello ESP32-S3!");
  
  tft.setTextColor(ST7735_GREEN);
  tft.setCursor(10, 30);
  tft.println("ST7735 TFT Display");
  
  // Рисование простых фигур
  tft.drawRect(5, 50, 30, 30, ST7735_RED);      // Прямоугольник
  tft.fillCircle(80, 45, 15, ST7735_BLUE);      // Круг
  tft.drawLine(0, 60, 127, 60, ST7735_YELLOW);  // Линия
}

int16_t cnt = 0;

void loop() {
  // Мигающий текст в цикле
  static uint32_t lastChange = 0;
  static bool state = false;

  generateRandomBackground();

  state = !state;
  lastChange = millis();
  
  tft.fillRect(10, 60, 100, 10, ST7735_BLACK);
  tft.setTextColor(state ? ST7735_CYAN : ST7735_MAGENTA);
  tft.setCursor(10, 60);
  tft.print("cnt: ");
  tft.print(cnt);
  cnt++;
  
  delay(5);
  
}
