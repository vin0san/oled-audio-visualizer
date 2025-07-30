#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define BUTTON_PIN     13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int barCount = 0;
bool isWaveMode = true;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  // Read button state
  bool currentButtonState = digitalRead(BUTTON_PIN);

  // Detect rising edge (button press)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    isWaveMode = !isWaveMode;  // Toggle mode
    delay(200);  // Simple debounce
  }
  lastButtonState = currentButtonState;

  display.clearDisplay();

  // Draw frame
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  // Mode label
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(4, 2);
  display.print("Mode: ");
  display.print(isWaveMode ? "Wave" : "Bars");

  // Visualizer
  if (isWaveMode) {
    // For cont. wave
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      int y = 32 + 15 * sin((x + barCount) * 0.2);
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  } else {
    // For bars
    int barWidth = 6;
    for (int i = 0; i < SCREEN_WIDTH / barWidth; i++) {
      int height = random(5, 30);
      int x = i * barWidth + 1;
      int yCenter = SCREEN_HEIGHT / 2;

      display.fillRect(x, yCenter - height, barWidth - 2, height, SSD1306_WHITE);
      display.fillRect(x, yCenter, barWidth - 2, height, SSD1306_WHITE);
    }
  }

  display.display();
  barCount++;
  delay(30);  // Smooth animation
}
