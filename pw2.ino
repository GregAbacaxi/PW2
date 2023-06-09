// + Bibliotecas
#include <Arduino.h>
#include <Wire.h>
#include <PCF8574.h>
#include <Servo.h>
#include <DFRobot_RGBLCD1602.h>

// + Pinos
  const int pushbutton = 8;
  const int buzzer = 3;
  const int echo = 12;
  const int trigger = 11;
  const int ck = 7;
  const int dt = A2;
  const int sw = A3;
  const int joyX = A1;
  const int joyY = A0;
  Servo myServo;
  PCF8574 pcb(0x20);
  DFRobot_RGBLCD1602 lcd(16, 2);

// + Variáveis
  int i;
  int ultPosicao;
  int leituraCLK;
  bool bCW;
  int posServo = 0;

  //// - Game
    int score;
    int highscore;
    int nivel;
    bool flag;

  //// - Aleatório
    int v_aleatoria;

  //// - Tempo
    long intervalo;
    long tempo_inicial;
    long tempo_final;

// + Protótipos de funções
long readUltrasonicDistance (int triggerPin, int echoPin);

void mostrarMenu();

void transitarNivel();

void gameOver();

bool desafioPushbutton ();

bool desafioDistancia ();

bool desafioEncoder ();

bool desafioJoystick ();

void escolherDesafio ();

// + Setup
  void setup() {
    //// - Pinmodes
      pinMode(pushbutton, INPUT_PULLUP);
      pinMode(buzzer, OUTPUT);
      pinMode(echo, INPUT);
      pinMode(trigger, OUTPUT);
      pinMode(ck, INPUT);
      pinMode(dt, INPUT);
      pinMode(sw, INPUT_PULLUP);
      pinMode(joyX, INPUT);
      pinMode(joyY, INPUT);

      myServo.attach(10);

      pcb.begin();

    //// - Serial
      Serial.begin(9600);

    //// - Variáveis
      randomSeed(analogRead(0));
      ultPosicao = digitalRead(ck);
      highscore = 5;

    //// - LCD
      lcd.init();
      lcd.setRGB(255, 255, 255);
      lcd.setCursor(0, 0);
      
  }

// + Loop
  void loop() {
    mostrarMenu();

    for(i = 0; i < 5 && flag; i++) {
      escolherDesafio();
    }

    if (flag) {
      pcb.write(4, LOW);
      pcb.write(5, LOW);
      pcb.write(6, HIGH);
      pcb.write(7, HIGH);
      transitarNivel();
    }

    for(i = 0; i < 5 && flag; i++) {
      escolherDesafio();
    }

    if (flag) {
      transitarNivel();
      pcb.selectNone();
    }

    while (flag) {
      intervalo <= 500 ? intervalo = intervalo : intervalo -= 100;
      posServo < 90 ? posServo = 180 : posServo = 0; 
      pcb.toggle(0);
      pcb.toggle(1);
      pcb.toggle(2);
      pcb.toggle(3);
      pcb.toggle(4);
      pcb.toggle(5);
      pcb.toggle(6);
      pcb.toggle(7);
      myServo.write(posServo);
      escolherDesafio();
    }

    gameOver();
  }

// + Declarações de funções
  //// - Ler distância
    long readUltrasonicDistance (int triggerPin, int echoPin) {
      pinMode(triggerPin, OUTPUT);
      digitalWrite(triggerPin, LOW);
      delayMicroseconds(2);
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
      pinMode(echoPin, INPUT);
      return pulseIn(echoPin, HIGH);
    }

  //// - Menu
    void mostrarMenu() {
      nivel = 0;
      score = 0;
      intervalo = 5000;
      flag = true;

      pcb.write(0, LOW);
      pcb.write(1, LOW);
      pcb.write(2, LOW);
      pcb.write(3, LOW);
      pcb.write(4, HIGH);
      pcb.write(5, HIGH);
      pcb.write(6, HIGH);
      pcb.write(7, HIGH);
      
      tone(buzzer, 1865); // A#
      delay(100);
      tone(buzzer, 1568); // G
      delay(100);
      tone(buzzer, 1397); // F
      delay(100);
      tone(buzzer, 1245); // D#
      delay(200);
      tone(buzzer, 1865); // A#
      delay(100);
      tone(buzzer, 1568); // G
      delay(100);
      tone(buzzer, 1397); // F
      delay(100);
      tone(buzzer, 1245); // D#
      delay(100);
      tone(buzzer, 1397); // F
      delay(200);
      noTone(buzzer);

      lcd.clear();
      lcd.setRGB(255, 255, 255);
      lcd.print("INICIO DE JOGO!");
      lcd.setCursor(0, 1);
      lcd.print("HIGHSCORE ATUAL: ");
      lcd.print(highscore);
      delay(1000);
      for (int i = 0; i <= 3; i++){
        lcd.scrollDisplayLeft();
        delay(500);
      }
      
      lcd.clear();
      lcd.setRGB(255, 255, 255);
      lcd.print("PRESSIONE O PUSHBUTTON PARA COMECAR!");
      delay(500);
      while (digitalRead(pushbutton)){
        lcd.scrollDisplayLeft();
        delay(200);
      }

    }

  //// - Transição
    void transitarNivel() {
      nivel++;
      intervalo == 5000 ? intervalo -= 1000 : intervalo = intervalo ;
      
      lcd.clear();
      switch (nivel){
        case 0:
          lcd.setRGB(0, 127, 255);
          break;
        case 1:
          lcd.setRGB(0, 255, 127);
          break;
        case 2:
          lcd.setRGB(255, 255, 0);
      }
      lcd.print("MAIS RAPIDO!!");

      tone(buzzer, 1047); // C
      delay(100);
      tone(buzzer, 1245); // D#
      delay(100);
      tone(buzzer, 1397); // F
      delay(100);
      tone(buzzer, 1568); // G
      delay(200);
      tone(buzzer, 1047); // C
      delay(100);
      tone(buzzer, 1245); // D#
      delay(100);
      tone(buzzer, 1397); // F
      delay(200);
      noTone(buzzer);
    }

  //// - Game Over
    void gameOver() {
      pcb.selectAll();
      lcd.clear();
      lcd.setRGB(255, 0, 0);
      lcd.print("GAME OVER!");

      tone(buzzer, 1865); // C
      delay(100);
      tone(buzzer, 1568); // A#
      delay(100);
      tone(buzzer, 1397); // C
      delay(100);
      tone(buzzer, 1245); // A#
      delay(100);
      tone(buzzer, 1865); // D
      delay(100);
      tone(buzzer, 1568); // A#
      delay(100);
      noTone(buzzer);
      delay(10);
      tone(buzzer, 1397); // A#
      delay(100);
      tone(buzzer, 1245); // C
      delay(100);
      tone(buzzer, 1397); // G
      delay(200);
      noTone(buzzer);

      if (score > highscore) {
        highscore = score;
        pcb.selectNone();
        lcd.clear();
        lcd.setRGB(127, 255, 0);
        lcd.print("PARABENS!");
        lcd.setCursor(0, 1);
        lcd.print("NOVO HIGHSCORE: ");
        lcd.print(highscore);
        delay(2000);
        for (int i = 0; i <= 20; i++){
          lcd.scrollDisplayLeft();
          delay(500);
        }

        tone(buzzer, 1047); // C
        delay(100);
        tone(buzzer, 1245); // D#
        delay(100);
        tone(buzzer, 1397); // F
        delay(100);
        tone(buzzer, 1568); // G
        delay(200);
        tone(buzzer, 1047); // C
        delay(100);
        tone(buzzer, 1245); // D#
        delay(100);
        tone(buzzer, 1397); // F
        delay(200);
        noTone(buzzer);

      }
    }

  //// - Desafios
    bool desafioPushbutton () {
      tempo_inicial = millis();
      lcd.clear();
      lcd.print("PRESSIONE O BOTAO!");
      lcd.setCursor(0, 1);
      lcd.print("SCORE: ");
      lcd.print(score);
      for (int i = 0; i <= 2; i++){
        lcd.scrollDisplayLeft();
        delay(200);
      }
      tone(buzzer, 1397); // F
      noTone(buzzer);
      while (digitalRead(pushbutton)) {
        tempo_final = millis();
        if (tempo_final - tempo_inicial == intervalo) {
          return false;
        }
      }
      score++;
      return true;
    }

    bool desafioDistancia () {
      tempo_inicial = millis();
      lcd.clear();
      lcd.print("SENSOR DE DISTANCIA!");
      lcd.setCursor(0, 1);
      lcd.print("SCORE: ");
      lcd.print(score);
      for (int i = 0; i <= 4; i++){
        lcd.scrollDisplayLeft();
        delay(200);
      }
      tone(buzzer, 1397); // F
      noTone(buzzer);
      while (0.0344/2 * readUltrasonicDistance(trigger, echo) >= 3) {
        tempo_final = millis();
        if (tempo_final - tempo_inicial == intervalo) {
          return false;
        }
      }
      score++;
      return true;
    }

    bool desafioEncoder () {
      v_aleatoria = random(0,3);
      tempo_inicial = millis();
      
      switch (v_aleatoria){
        case 0:  // Click
          lcd.clear();
          lcd.print("PRESSIONE O ENCODER!");
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          for (int i = 0; i <= 4; i++){
            lcd.scrollDisplayLeft();
            delay(200);
          }
          tone(buzzer, 1397); // F
          noTone(buzzer);

          while (true) {
            tempo_final = millis();
            if (tempo_final - tempo_inicial == intervalo) {
              return false;
            }

            if (analogRead(sw) == 0) {
              score++;
              return true;
            }
          
          }
          break;
        case 1:  // Sentido Horário
          lcd.clear();
          lcd.print("ENCODER HORARIO!");
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          tone(buzzer, 1397); // F
          noTone(buzzer);
          
          while (true) {
            tempo_final = millis();
            if (tempo_final - tempo_inicial == intervalo) {
              return false;
            }

            leituraCLK = digitalRead(ck);
            if (leituraCLK != ultPosicao){
              if (digitalRead(dt) != leituraCLK){
                score++;
                return true;
              }
            }
            
            ultPosicao = leituraCLK;
          }
          break;
        case 2:  // Sentido Anti-horário
          lcd.clear();
          lcd.print("ENCODER ANTI-HORARIO!");
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          for (int i = 0; i <= 5; i++){
            lcd.scrollDisplayLeft();
            delay(200);
          }
          tone(buzzer, 1397); // F
          noTone(buzzer);
          
          while (true) {
            tempo_final = millis();
            if (tempo_final - tempo_inicial == intervalo) {
              return false;
            }

            leituraCLK = digitalRead(ck);
            if (leituraCLK != ultPosicao){
              if (digitalRead(dt) == leituraCLK){
                score++;
                return true;
              }
            }
            
            ultPosicao = leituraCLK;
          }
          break;
        default:
          lcd.clear();
          lcd.print("ERRO");
          flag = false;
          break;
      }
    }

    bool desafioJoystick () {
      v_aleatoria = random(4);
      tempo_inicial = millis();

      switch (v_aleatoria){
        case 0:
          lcd.clear();
          lcd.print("JOYSTICK ESQUERDA!");
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          for (int i = 0; i <= 2; i++){
            lcd.scrollDisplayLeft();
            delay(200);
          }
          tone(buzzer, 1397); // F
          noTone(buzzer);
          while (analogRead(joyX) > 0) {
            tempo_final = millis();
            if (tempo_final - tempo_inicial == intervalo) {
              return false;
            }
          }
          score++;
          return true;
          break;
        case 1:
          lcd.clear();
          lcd.print("JOYSTICK DIREITA!");
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          lcd.scrollDisplayLeft();
          delay(200);
          
          tone(buzzer, 1397); // F
          noTone(buzzer);
          
          while (analogRead(joyX) < 1023) {
            tempo_final = millis();
            if (tempo_final - tempo_inicial == intervalo) {
              return false;
            }
          }
          score++;
          return true;
          break;
        case 2:
          lcd.clear();
          lcd.print("JOYSTICK BAIXO!");
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          tone(buzzer, 1397); // F
          noTone(buzzer);
          while (analogRead(joyY) > 0) {
            tempo_final = millis();
            if (tempo_final - tempo_inicial == intervalo) {
              return false;
            }
          }
          score++;
          return true;
          break;
        case 3:
          lcd.clear();
          lcd.print("JOYSTICK CIMA!");
          lcd.setCursor(0, 1);
          lcd.print("SCORE: ");
          lcd.print(score);
          tone(buzzer, 1397); // F
          noTone(buzzer);
          while (analogRead(joyY) < 1023) {
            tempo_final = millis();
            if (tempo_final - tempo_inicial == intervalo) {
              return false;
            }
          }
          score++;
          return true;
          break;
        default:
          lcd.clear();
          lcd.print("ERRO");
          flag = false;
          break;
      }
    }

  //// - Escolha
    void escolherDesafio () {
      nivel >= 1 ? v_aleatoria = random(4) : v_aleatoria = random(3);

      switch (v_aleatoria){
        case 0:
          flag = desafioPushbutton();
          break;
        case 1:
          flag = desafioDistancia();
          break;
        case 2:
          flag = desafioEncoder();
          break;
        case 3:
          flag = desafioJoystick();
          break;
        default :
          lcd.clear();
          lcd.print("ERRO");
          flag = false;
          break;
      }
    }
