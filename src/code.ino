#include <Wire.h>                // Biblioteca para comunicação com o Display
#include <LiquidCrystal_I2C.h>   // Biblioteca específica para o Display LCD

// --- Definição dos Pinos ---
#define ledVm    3               // Luz vermelha no pino 3
#define ledVd    4               // Luz verde no pino 4
#define echoPin  6               // Recebe o som do sensor de distância
#define trigPin  7               // Envia o som do sensor de distância
#define emergencyButton 8        // Botão de emergência no pino 8
#define transistorMotor 9        // Controle do motor no pino 9 (com velocidade ajustável)

// Configura o display LCD (Endereço 0x27, 16 colunas e 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variável de "memória": 0 para sistema normal, 1 para emergência travada
bool emergencyState = 0; 

void setup() {
  // Configura se os pinos são de saída (enviam energia) ou entrada (recebem sinal)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVm, OUTPUT);
  pinMode(ledVd, OUTPUT);
  pinMode(emergencyButton, INPUT_PULLUP); // Botão usa resistor interno do Arduino
  pinMode(transistorMotor, OUTPUT);
  
  // Inicialização do visor LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Mensagem de boas-vindas ao ligar o sistema
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(1000); // Espera 1 segundo
  lcd.clear();
  
  Serial.begin(9600); // Abre a comunicação com o computador para testes
}
 
void loop() {
  // --- PARTE 1: VERIFICAÇÃO DO BOTÃO ---
  // Se o botão for pressionado (sinal 1)...
  if (!digitalRead(emergencyButton)) {
    emergencyState = !emergencyState; // Inverte o estado: se estava livre, trava; se estava travado, libera.
    
    // Trava o código aqui até você soltar o botão (evita que ele mude de estado mil vezes num clique só)
    while(digitalRead(emergencyButton) == 0); 
    delay(50); // Pequena pausa para evitar ruídos elétricos
  }

  // --- PARTE 2: MODO DE EMERGÊNCIA (Sistema Travado) ---
  if (emergencyState == 1) {
    analogWrite(transistorMotor, 0);  // Desliga o motor imediatamente
    digitalWrite(ledVm, 1);           // Acende o sinal vermelho
    digitalWrite(ledVd, 0);           // Apaga o sinal verde
    
    // Exibe o aviso de perigo no visor
    lcd.setCursor(0, 0);
    lcd.print(" EMERGENCY STOP ");
    lcd.setCursor(0, 1);
    lcd.print(" SYSTEM LOCKED  "); 
  } 
  
  // --- PARTE 3: MODO NORMAL (A porta funciona com o sensor) ---
  else {
    long duration, distance;
    
    // O sensor envia um pulso de som (como um morcego) para medir a distância
    digitalWrite(trigPin, 0);
    delayMicroseconds(2);
    digitalWrite(trigPin, 1);
    delayMicroseconds(10);
    digitalWrite(trigPin, 0);
    
    // Mede quanto tempo o som demorou para voltar e calcula a distância em cm
    duration = pulseIn(echoPin, 1);
    distance = (duration / 2) / 29.1;
    
    // Se algo estiver a menos de 100cm (Porta Liberada)
    if(distance < 100){
      digitalWrite(ledVm, 0);
      digitalWrite(ledVd, 1);           // Luz verde acesa
      analogWrite(transistorMotor, 25); // Liga o motor em velocidade baixa
      lcd.setCursor(0, 0);
      lcd.print(" DOOR UNLOCKED  ");
    }
    // Se não houver ninguém perto (Porta Fechada)
    else{
      digitalWrite(ledVd, 0);
      digitalWrite(ledVm, 1);           // Luz vermelha acesa
      analogWrite(transistorMotor, 0);  // Motor parado
      lcd.setCursor(0, 0);
      lcd.print("  DOOR CLOSED   ");
    }
    
    // Mostra a distância em tempo real na segunda linha do visor
    lcd.setCursor(0, 1);
    lcd.print("Distance: "); 
    lcd.print(distance);
    lcd.print(" cm  "); // Espaços extras para limpar números antigos
    
    delay(100); // Pequena pausa antes de ler tudo de novo
  }
}