#include <Arduino.h>
//Definição dos pinos de controle do motor
const int motorA = 5; //velocidade motor A - de 0 a 255
const int motorB = 6; //velocidade motor B - de 0 a 255
const int dirA = 7; //direcao do motor A - HIGH ou LOW
const int dirB = 8; //direcao do motor B - HIGH ou LOW
//Definição dos pinos dos sensores
const int pin_S1 = 9;
const int pin_S2 = 10;
const int pin_S3 = 11;
const int pin_S4 = 12;
const int pin_S5 = 13;
const int pin_obstacle = 2;
const int pin_button = 3;

bool sensor1 = 0;
bool sensor2 = 0;
bool sensor3 = 0;
bool sensor4 = 0;
bool sensor5 = 0;
bool sensors = 0;
bool obstacle = 0;
bool button = 0;
int v1_star = 0;
int v2_star = 0;

void setup(){
    //Setamos os pinos de controle dos motores como saída
    pinMode(motorA, OUTPUT);
    pinMode(motorB, OUTPUT);
    pinMode(dirA, OUTPUT);
    pinMode(dirB, OUTPUT);
    //Setamos a direção inicial do motor como 0, isso fará com que ambos os motores girem para frente
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, LOW);
    //Setamos os pinos dos sensores como entrada
    pinMode(pin_S1, INPUT);
    pinMode(pin_S2, INPUT);
    pinMode(pin_S3, INPUT);
    pinMode(pin_S4, INPUT);
    pinMode(pin_S5, INPUT);
    pinMode(pin_aprox, INPUT);
    pinMode(pin_button, INPUT);
    button = 0;
}

void loop(){
    //Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.
    sensor1 = digitalRead(pin_S1);
    sensor2 = digitalRead(pin_S2);
    sensor3 = digitalRead(pin_S3);
    sensor4 = digitalRead(pin_S4);
    sensor5 = digitalRead(pin_S5);
    obstacle = digitalRead(pin_obstacle);
    v1_star = analogRead(motorA);
    v2_star = analogRead(motorB);

    if (button == 0) {
        button = digitalRead(pin_button);
    }

    if ((sensor1 + sensor2 + sensor3 + sensor4 + sensor5) > 0){
        sensors = 0;
    }

    else {
        sensors = 0;
        button = 0;
    }

    //Aqui está toda a lógica de comportamento do robô: Para a cor branca atribuímos o valor 0 e, para a cor preta, o valor 1.
    if ((sensors == 0) || (button == 0)) {
        analogWrite(motorA, 0);
        analogWrite(motorB, 0);
        button = 0;
    }

    else if (obstacle == 0) {
        analogWrite(motorA, 0);
        analogWrite(motorB, 0);
    }

    else {
        if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            analogWrite(motorA, 0);
            analogWrite(motorB, 0);
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == HIGH)) {
            analogWrite(motorA, 255);
            analogWrite(motorB, 0);
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == HIGH) && (sensor5 == LOW)) {
            analogWrite(motorA, 255);
            analogWrite(motorB, 127);
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == HIGH) && (sensor5 == HIGH)) {
            analogWrite(motorA, 255);
            analogWrite(motorB, 64);
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == HIGH) && (sensor4 == LOW) && (sensor5 == LOW)) {
            analogWrite(motorA, 255);
            analogWrite(motorB, 255);
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == HIGH) && (sensor4 == HIGH) && (sensor5 == LOW)) {
            analogWrite(motorA, 255);
            analogWrite(motorB, 191);
        }
        else if ((sensor1 == LOW) && (sensor2 == HIGH) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            analogWrite(motorA, 127);
            analogWrite(motorB, 255);
        }
        else if ((sensor1 == LOW) && (sensor2 == HIGH) && (sensor3 == HIGH) && (sensor4 == LOW) && (sensor5 == LOW)) {
            analogWrite(motorA, 191);
            analogWrite(motorB, 255);
        }
        else if ((sensor1 == HIGH) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            analogWrite(motorA, 0);
            analogWrite(motorB, 255);
        }
        else if ((sensor1 == HIGH) && (sensor2 == HIGH) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            analogWrite(motorA, 64);
            analogWrite(motorB, 255);
        }
        else {
            analogWrite(motorA, v1_star);
            analogWrite(motorB, v2_star);
        }
    }
}