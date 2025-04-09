#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


#define relayPin 2
#define pbPin 5
#define floatPin 7

uint8_t filling_status = 0;

bool display_toggle = false;
bool floater_toggle = false;


void setup(void) {
  Serial.begin(9600);

  lcd.init();  // initialize the lcd
  lcd.backlight();
  lcd.clear();

  pinMode(pbPin, INPUT_PULLUP);
  pinMode(floatPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);


  relay_Deactivate();

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);
}

void loop() {
  if (!digitalRead(pbPin)) {
    delay(100);
    while (!digitalRead(pbPin)) {
      delay(50);
    }

    if (filling_status == 0) {
      filling_status = 1;
      display_toggle = false;
    } else if (filling_status == 2) {
      filling_status = 0;
      display_toggle = false;
    }
  }

  if (!digitalRead(floatPin) && filling_status == 1 && !floater_toggle) {
    delay(100);
    filling_status = 2;
    display_toggle = false;
    floater_toggle = true;
    relay_Deactivate();
  }

  if (digitalRead(floatPin)) {
    floater_toggle = false;
  }


  if (filling_status == 0 && !display_toggle) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESS the Button");
    lcd.setCursor(0, 1);
    lcd.print("to refill...");
    display_toggle = true;
  } else if (filling_status == 1 && !display_toggle) {
    relay_Activate();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("refilling...");
    lcd.setCursor(0, 1);
    lcd.print("please wait...");
    display_toggle = true;
  } else if (filling_status == 2 && !display_toggle) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("refilling done");
    lcd.setCursor(0, 1);
    lcd.print("PRESS 2 continue");
    display_toggle = true;
  }
}

void relay_Activate() {
  digitalWrite(relayPin, 0);
}

void relay_Deactivate() {
  digitalWrite(relayPin, 1);
}
