#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ledVm 3
#define ledVd 4
#define echoPin 6
#define trigPin 7
#define emergencyButton 8
#define transistorMotor 9

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool emergencyState = 0;

int doorState = 0;
unsigned long timerMillis = 0; 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVm, OUTPUT);
  pinMode(ledVd, OUTPUT);
  pinMode(emergencyButton, INPUT_PULLUP);
  pinMode(transistorMotor, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();
  
  Serial.begin(9600);
}

void loop() {
  if (!digitalRead(emergencyButton)) {
    emergencyState = !emergencyState;
    while(digitalRead(emergencyButton) == 0);
    delay(50);
    
    if (emergencyState == 1) {
      analogWrite(transistorMotor, 0);
      doorState = 0; 
    }
  }

  if (emergencyState == 1) {
    digitalWrite(ledVm, 1);
    digitalWrite(ledVd, 0);
    
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(" EMERGENCY STOP ");
    lcd.setCursor(0, 1);
    lcd.print(" SYSTEM LOCKED  ");
  } 
  
  else {
    long duration, distance;
    
    digitalWrite(trigPin, 0);
    delayMicroseconds(2);
    digitalWrite(trigPin, 1);
    delayMicroseconds(10);
    digitalWrite(trigPin, 0);
    
    duration = pulseIn(echoPin, 1);
    distance = (duration / 2) / 29.1;
    
    lcd.setCursor(0, 1);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm  ");

    switch (doorState) {
      
      case 0: // ESTADO 0: PORTA FECHADA
        digitalWrite(ledVd, 0);
        digitalWrite(ledVm, 1);
        analogWrite(transistorMotor, 0);
        lcd.setCursor(0, 0);
        lcd.print("  DOOR CLOSED   ");

        if (distance < 100) {
          doorState = 1; 
          timerMillis = millis(); 
        }
        break;

      case 1: // ESTADO 1: PORTA ABRINDO
        digitalWrite(ledVm, 0);
        digitalWrite(ledVd, 1);
        analogWrite(transistorMotor, 25);
        lcd.setCursor(0, 0);
        lcd.print(" DOOR OPENING   ");

        while(millis() - timerMillis < 2000){}

        if (millis() - timerMillis >= 2000) {
          analogWrite(transistorMotor, 0); 
          doorState = 2; 
        }
        break;

      case 2: // ESTADO 2: PORTA ABERTA
        lcd.setCursor(0, 0);
        lcd.print(" DOOR UNLOCKED  ");
      
        if (distance >= 100) {
          doorState = 3; 
          timerMillis = millis();
        }
        break;

      case 3: //ESTADO 3: PORTA FECHANDO
        digitalWrite(ledVm, 1);
        digitalWrite(ledVd, 0);
        analogWrite(transistorMotor, 25);
        lcd.setCursor(0, 0);
        lcd.print(" DOOR CLOSING   ");

        while(millis() - timerMillis < 2000){}

        if (millis() - timerMillis >= 2000) {
          analogWrite(transistorMotor, 0);
          doorState = 0;
        }
        break;
    }
    
    delay(50); 
  }
}