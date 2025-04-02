#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

Adafruit_ADS1015 ads;  // Create an ADS1015 object

#define relayPin 2
#define pbPin 5

bool fillingStatus = false;
int16_t raw = 0;
uint16_t base = 0;
uint8_t toFill = 1;

void setup(void) {
  Serial.begin(9600);
  ads.begin();                // Initialize the ADS1015
  ads.setGain(GAIN_SIXTEEN);  // ±2.048V range (1 bit = 1mV for ADS1015)

  lcd.init();  // initialize the lcd
  lcd.backlight();
  // lcd.setCursor(0, 0);
  // lcd.print("ADC Value:");

  pinMode(pbPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  relay_Deactivate();

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);
  for (int a = 0; a <= 4; a++) {
    base = base + ads.readADC_Differential_0_1();
    delay(30);
  }
  base = base / 5;
}

void loop(void) {
  for (int a = 0; a < 4; a++) {
    raw = raw + ads.readADC_Differential_0_1();
    delay(30);
  }
  raw = raw / 5;


  if (!digitalRead(pbPin)) {
    uint32_t pb_count = 0;
    delay(100);
    while (!digitalRead(pbPin)) {
      pb_count++;
      delay(100);
      if (pb_count > 30) {
        fillingStatus = true;
        break;
      }
    }

    if (pb_count < 15) {
      toFill++;
      if (toFill > 3) {
        toFill = 1;
      }
    }
  }

  if (fillingStatus) {
    delay(300);
    relay_Activate();
    lcd.setCursor(0, 0);
    lcd.print("Filling...      ");
    delay(95000 * toFill);
    relay_Deactivate();
    fillingStatus = false;
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Press to Fill");
  }

  // if (fillingStatus) {
  //   relay_Activate();
  //   lcd.setCursor(0, 0);
  //   lcd.print("Filling...      ");

  //   delay(300);
  // } else {
  //   relay_Deactivate();
  //   lcd.setCursor(0, 0);
  //   lcd.print("Press to Fill");
  // }

  // if (raw >= (base + 1) || raw <= (base - 1)) {
  //   fillingStatus = false;
  // }

  // lcd.setCursor(0, 1);
  // lcd.print(raw);
  // lcd.setCursor(12, 1);
  // lcd.print(base);

  lcd.setCursor(0, 1);
  lcd.print("To Fill:");
  lcd.setCursor(8, 1);
  lcd.print(toFill);
}

void relay_Activate() {
  digitalWrite(relayPin, 0);
}

void relay_Deactivate() {
  digitalWrite(relayPin, 1);
}
