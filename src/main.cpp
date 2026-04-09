/*
  Autor: Kauã Cordeiro
  Programa: Sistema de Porta Automática
  Descrição: Projeto de um sistema de porta inteligente utilizando um sensor ultrassônico, um display LCD, LEDs indicadores e um motor controlado por um transistor. O sistema possui também um botão de emergência para interromper o funcionamento em caso de necessidade.
  Versão: 1.3
  Data: 09/04/2026
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// PROTÓTIPO DAS FUNÇÕES
void processarBotaoEmergencia(bool, bool &, bool &);
void processarModoEmergencia(bool, int &, int, int, int);
void calculoDistancia(long &, long &, int, int);
void controlePorta(unsigned long, unsigned long &, int &, long, int, int, int);

// CONSTANTES
const int ledVermelho = 3;
const int ledVerde = 4;
const int echoPino = 6;
const int trigPino = 7;
const int botaoEmergencia = 8;
const int transistorMotor = 9;

void setup()
{
  // DEFINIÇÃO SAÍDAS/ENTRADAS
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(trigPino, OUTPUT);
  pinMode(echoPino, INPUT);
  pinMode(botaoEmergencia, INPUT_PULLUP);
  pinMode(transistorMotor, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.print("Starting...");
  delay(1000);
  lcd.clear();
}

void loop()
{
  // VARIÁVEIS
  unsigned long agora = millis();
  static unsigned long tempoInicial = 0;

  bool estadoBotaoEmergencia = !digitalRead(botaoEmergencia);
  static bool estadoAnteriorBotaoEmergencia = false;
  static bool modoEmergencia = false;

  static int estadoPorta = 0;
  long distancia, duracao;

  // PROCESSAMENTO BOTÃO EMERGÊNCIA
  processarBotaoEmergencia(estadoBotaoEmergencia, estadoAnteriorBotaoEmergencia, modoEmergencia);

  // MODO EMERGENCIA
  if (modoEmergencia)
    processarModoEmergencia(modoEmergencia, estadoPorta, transistorMotor, ledVermelho, ledVerde);

  // MODO PADRÃO
  else
  {
    // CALCULO DA DISTÂNCIA
    calculoDistancia(distancia, duracao, trigPino, echoPino);

    // EXIBE DISTÂNCIA
    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distancia);
    lcd.print(" cm    ");

    // CONTROLE DOS ESTADOS DA PORTA
    controlePorta(agora, tempoInicial, estadoPorta, distancia, ledVermelho, ledVerde, transistorMotor);
  }
}

void processarBotaoEmergencia(bool estadoBotaoEmergencia, bool &estadoAnteriorBotaoEmergencia, bool &modoEmergencia)
{
  if (estadoBotaoEmergencia && !estadoAnteriorBotaoEmergencia)
  {
    modoEmergencia = !modoEmergencia;
    lcd.clear();
  }
  estadoAnteriorBotaoEmergencia = estadoBotaoEmergencia;
}

void processarModoEmergencia(bool modoEmergencia, int &estadoPorta, int transistorMotor, int ledVermelho, int ledVerde)
{
  analogWrite(transistorMotor, 0);
  estadoPorta = 0;
  digitalWrite(ledVermelho, 1);
  digitalWrite(ledVerde, 0);

  lcd.setCursor(0, 0);
  lcd.print(" EMERGENCY STOP ");
  lcd.setCursor(0, 1);
  lcd.print(" SYSTEM LOCKED  ");
}

void calculoDistancia(long &distancia, long &duracao, int trigPino, int echoPino)
{
  // LEITURA SENSOR ULTRASSÔNICO
  digitalWrite(trigPino, 0);
  delayMicroseconds(2);
  digitalWrite(trigPino, 1);
  delayMicroseconds(10);
  digitalWrite(trigPino, 0);

  duracao = pulseIn(echoPino, 1);
  distancia = (duracao / 2) / 29.1;
}

void controlePorta(unsigned long agora, unsigned long &tempoInicial, int &estadoPorta, long distancia, int ledVermelho, int ledVerde, int transistorMotor)
{
  switch (estadoPorta)
  {
  case 0: // FECHADA
    digitalWrite(ledVerde, 0);
    digitalWrite(ledVermelho, 1);
    analogWrite(transistorMotor, 0);

    lcd.setCursor(0, 0);
    lcd.print("  DOOR CLOSED   ");

    // ABRE SE DISTÂNCIA MENOR QUE 100cm
    if (distancia < 100)
    {
      estadoPorta = 1;
      tempoInicial = agora;
    }
    break;

  case 1: // ABRINDO
    digitalWrite(ledVermelho, 0);
    digitalWrite(ledVerde, 1);
    analogWrite(transistorMotor, 200);

    lcd.setCursor(0, 0);
    lcd.print(" DOOR OPENING   ");

    if (agora - tempoInicial >= 2000)
    {
      analogWrite(transistorMotor, 0);
      estadoPorta = 2;
    }
    break;

  case 2: // ABERTA
    lcd.setCursor(0, 0);
    lcd.print(" DOOR UNLOCKED  ");

    // FECHA SE DISTÂNCIA MAIOR QUE 100cm
    if (distancia >= 100)
    {
      estadoPorta = 3;
      tempoInicial = agora;
    }
    break;

  case 3: // FECHANDO
    digitalWrite(ledVermelho, 1);
    digitalWrite(ledVerde, 0);
    analogWrite(transistorMotor, 200);

    lcd.setCursor(0, 0);
    lcd.print(" DOOR CLOSING   ");

    if (agora - tempoInicial >= 2000)
    {
      analogWrite(transistorMotor, 0);
      estadoPorta = 0;
    }
    break;
  }
  delay(50);
}