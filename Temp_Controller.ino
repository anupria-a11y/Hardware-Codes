#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define ENA 10
#define IN1 8
#define IN2 9

LiquidCrystal_I2C lcd(0x27, 16, 2); 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Temp & Fan Ctrl");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    lcd.setCursor(0, 0);
    lcd.print("Error reading");
    lcd.setCursor(0, 1);
    lcd.print("DHT11 Sensor");
    delay(1000);
    return;
  }

  int fanSpeed = map(temperature, 20, 40, 0, 255);
  fanSpeed = constrain(fanSpeed, 0, 255);
  analogWrite(ENA, fanSpeed);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223); // degree symbol
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  lcd.print("Fan: ");
  lcd.print(map(fanSpeed, 0, 255, 0, 100)); 
  lcd.print("%   ");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C  | Fan Speed: ");
  Serial.println(fanSpeed);

  delay(1000);
}