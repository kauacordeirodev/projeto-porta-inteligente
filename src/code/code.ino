#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ledVm    3
#define ledVd    4

#define echoPin  6
#define trigPin  7

#define emergencyButton 8
#define transistorMotor 9

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool emergencyState = 0; 

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
  if (digitalRead(emergencyButton) == 0) {
    
    emergencyState = !emergencyState; 
    
    while(digitalRead(emergencyButton) == 0); 
    delay(50);
  }


  if (emergencyState == 1) {
    analogWrite(transistorMotor, 0); 
    digitalWrite(ledVm, 1);         
    digitalWrite(ledVd, 0);
    
   
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
    distance = (duration / 2)/ 29.1;
    
    if(distance < 100){
      digitalWrite(ledVm, 0);
      digitalWrite(ledVd, 1);
      analogWrite(transistorMotor, 25);
      lcd.setCursor(0, 0);
      lcd.print(" DOOR UNLOCKED  ");
    }
    else{
      digitalWrite(ledVd, 0);
      digitalWrite(ledVm, 1);
      analogWrite(transistorMotor, 0);
      lcd.setCursor(0, 0);
      lcd.print(" DOOR CLOSED  ");
    }
    
    lcd.setCursor(0, 1);
    lcd.print("Distance: "); 
    lcd.print(distance);
    lcd.print(" cm ");
    
    delay(100); 
  }
}