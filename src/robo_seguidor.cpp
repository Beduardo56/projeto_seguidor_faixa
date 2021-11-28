// importando a biblioteca Arduino a qual contem as classes a serem utilizadas no codigo.
#include <Arduino.h>

// Definição global dos pinos de controle do motor.

// Velocidade motor A (esquerdo) - de 0 a 255, pino dital-PWM 5 do arduino UNO.
const int motorA = 5;
// Velocidade motor B (direito)- de 0 a 255, pino digital-PWM 6 do arduino UNO.
const int motorB = 6;
// Direcao do motor A - HIGH ou LOW, pino dital-PWM 7 do arduino UNO.
const int dirA = 7;
// Direcao do motor B - HIGH ou LOW, pino dital-PWM 8 do arduino UNO.
const int dirB = 8;

// Definição global dos pinos dos sensores.
// Pinos digitais 9-13 dedicados aos sensores infravermelho. 
const int pin_S1 = 9;
const int pin_S2 = 10;
const int pin_S3 = 11;
const int pin_S4 = 12;
const int pin_S5 = 13;

// Definindo pino digital 2 para ser usado pelo sensor de aproximação.
const int pin_obstacle = 2;
// Definindo pino digital 3 para ser usado pelo botão de inicialização do trajeto.
const int pin_button = 3;

// Declarando as variaveis globais que serão usadas na maquina de estado,
// são essas variaveis que receberão os estados dos pinos definidos acima.

// Declarando as variaveis dos sensores individuais, o valor padrão dessas variaveis é False.
bool sensor1 = 0;
bool sensor2 = 0;
bool sensor3 = 0;
bool sensor4 = 0;
bool sensor5 = 0;
// Declarando a variavel sensors, variavel que é dependente do valor de sensor1, sensor2, ... , sensor5. default=True.
bool sensors = 0;
// Declarando a variavel obstacle, variavel que recebe o valor do pino 2 digital.
bool obstacle = 0;
// Declarando a variavel button, variavel que recebe o valor do pino 3 digital.
bool button = 0;
// Declarando as variavels vx_star, essas variaveis guardam o estado anterior dos motores 1 e 2.
int v1_star = 0;
int v2_star = 0;

void setup(){
    // Definição do estado logico quando inicializamos o firmware do arduino.
    // Setando os pinos de controle dos motores como saída.
    pinMode(motorA, OUTPUT);
    pinMode(motorB, OUTPUT);
    pinMode(dirA, OUTPUT);
    pinMode(dirB, OUTPUT);
    // Setando a direção inicial do motor como 0, isso fará com que ambos os motores girem para frente.
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, LOW);
    //Setamos os pinos dos sensores como entrada.
    pinMode(pin_S1, INPUT);
    pinMode(pin_S2, INPUT);
    pinMode(pin_S3, INPUT);
    pinMode(pin_S4, INPUT);
    pinMode(pin_S5, INPUT);
    pinMode(pin_aprox, INPUT);
    pinMode(pin_button, INPUT);
    // Variavel do botão de inicialização será False inicialmente.
    button = 0;
    v1_star = 0;
    v2_star = 0;
}

void loop(){
    // Logica principal do arduino
    // Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.
    sensor1 = digitalRead(pin_S1);
    sensor2 = digitalRead(pin_S2);
    sensor3 = digitalRead(pin_S3);
    sensor4 = digitalRead(pin_S4);
    sensor5 = digitalRead(pin_S5);
    obstacle = digitalRead(pin_obstacle);

    // Se o botão no estado anterior estiver em False, então podemos ler o valor dessa variavel.
    // Isso faz com que o valor do botão so vá de True para False caso o seguidor termine o trajeto.
    if (button == 0) {
        button = digitalRead(pin_button);
    }
    // Logica para definirmos o valor da variavel sensors, se um dos sensores estiver detectando uma linha preta,
    // então essa variavel será True, se não False.
    if ((sensor1 + sensor2 + sensor3 + sensor4 + sensor5) > 0){
        sensors = 1;
    }
    else {
        sensors = 0;
        button = 0;
    }

    // Aqui está toda a lógica de comportamento do robô.
    // Caso não inicializamos o botão ou se os sensores não detectaram a linha preta, então entremos no estado de Idle.
    if ((sensors == 0) || (button == 0)) {
        // No estado de Idle o carrinho tem que parar e esperar a sua renicialização.
        analogWrite(motorA, 0);
        analogWrite(motorB, 0);
        button = 0;
        v1_star = 0;
        v2_star = 0;
    }
    // Caso não estejamos no estado Idle e a variavel de obstaculo esteja ativa, entao entremos no estado Stop.
    else if (obstacle == 1) {
        // No estado de Stop o carrinho tem que parar e esperar que o obstaculo saia da frente do carrinho.
        analogWrite(motorA, 0);
        analogWrite(motorB, 0);
        v1_star = 0;
        v2_star = 0;
    }
    // Caso não estivermos em Idle nem em Stop, então entraremos no estado Following.
    else {
        // No estado de Following, o motor irá ter seu comportamento controlado pelas variaveis individuais dos sensores infravermelhos.
        if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            // Estado em que todos os sensores estão em nivel baixo, o motor tem que para automaticamente.
            analogWrite(motorA, 0);
            analogWrite(motorB, 0);
            button = 0;
            v1_star = 0;
            v2_star = 0;
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == HIGH)) {
            // Estado em que somente o ultimo sensor a direita detecta a linha.
            // Nesse estado queremos que o motor faça uma curva acentuada para a esquerda.
            // 255 representa um duty cycle de 100% no PWM,
            // ou seja o motorA irá rodar com 100 % da sua velocidade e motorB com 0% da sua velocidade maxima.
            analogWrite(motorA, 255);
            analogWrite(motorB, 0);
            v1_star = 255;
            v2_star = 0;
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == HIGH) && (sensor5 == LOW)) {
            // Estado em que somente o penultimo sensor a direita detecta a linha.
            // Nesse estado queremos que o motor faça uma curva acentuada para a esquerda.
            // 255 representa um duty cycle de 100% no PWM, 127 representa um duty cycle de 50% no PWM
            // ou seja o motorA irá rodar com 100 % da sua velocidade e motorB com 50% da sua velocidade maxima.
            analogWrite(motorA, 255);
            analogWrite(motorB, 127);
            v1_star = 255;
            v2_star = 127;
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == HIGH) && (sensor5 == HIGH)) {
            // Estado em que o penultimo e o ultimo sensor a direita detecta a linha.
            // Nesse estado queremos que o motor faça uma curva acentuada para a esquerda.
            // 255 representa um duty cycle de 100% no PWM, 64 representa um duty cycle de 25% no PWM
            // ou seja o motorA irá rodar com 100 % da sua velocidade e motorB com 25% da sua velocidade maxima.
            analogWrite(motorA, 255);
            analogWrite(motorB, 64);
            v1_star = 255;
            v2_star = 64;
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == HIGH) && (sensor4 == LOW) && (sensor5 == LOW)) {
            // Estado em que somente o sensor do meio esta detectando a linha preta.
            // Nesse estado queremos que o motor siga em frente.
            // 255 representa um duty cycle de 100% no PWM
            // ou seja o motorA irá rodar com 100 % da sua velocidade e motorB com 100% da sua velocidade maxima.
            analogWrite(motorA, 255);
            analogWrite(motorB, 255);
            v1_star = 255;
            v2_star = 255;
        }
        else if ((sensor1 == LOW) && (sensor2 == LOW) && (sensor3 == HIGH) && (sensor4 == HIGH) && (sensor5 == LOW)) {
            // Estado em que  o sensor do meio e o penultimo sensor a direita estão detectando a linha preta.
            // Nesse estado queremos que faça uma ligeira curva a esquerda.
            // 255 representa um duty cycle de 100% no PWM e 191 representa 75% no PWM
            // ou seja o motorA irá rodar com 100 % da sua velocidade e motorB com 75% da sua velocidade maxima.
            analogWrite(motorA, 255);
            analogWrite(motorB, 191);
            v1_star = 255;
            v2_star = 191;
        }
        else if ((sensor1 == LOW) && (sensor2 == HIGH) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            // Estado em que somente o penultimo sensor a esquerda esta detectando a linha preta.
            // Nesse estado queremos que faça uma curva a direita.
            // 255 representa um duty cycle de 100% no PWM e 127 representa 50% no PWM
            // ou seja o motorA irá rodar com 50 % da sua velocidade e motorB com 100% da sua velocidade maxima.
            analogWrite(motorA, 127);
            analogWrite(motorB, 255);
            v1_star = 127;
            v2_star = 255;
        }
        else if ((sensor1 == LOW) && (sensor2 == HIGH) && (sensor3 == HIGH) && (sensor4 == LOW) && (sensor5 == LOW)) {
            // Estado em que o penultimo sensor a esquerda e o sensor do meio estão detectando a linha preta.
            // Nesse estado queremos que faça uma ligeira curva a direita.
            // 255 representa um duty cycle de 100% no PWM e 191 representa 75% no PWM
            // ou seja o motorA irá rodar com 75 % da sua velocidade e motorB com 100% da sua velocidade maxima.
            analogWrite(motorA, 191);
            analogWrite(motorB, 255);
            v1_star = 191;
            v2_star = 255;
        }
        else if ((sensor1 == HIGH) && (sensor2 == LOW) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            // Estado em que o ultimo sensor a esquerda esta detectando a linha preta.
            // Nesse estado queremos que faça uma curva acentuada a direita.
            // 255 representa um duty cycle de 100% no PWM.
            // ou seja o motorA irá rodar com 0 % da sua velocidade e motorB com 100% da sua velocidade maxima.
            analogWrite(motorA, 0);
            analogWrite(motorB, 255);
            v1_star = 0;
            v2_star = 255;
        }
        else if ((sensor1 == HIGH) && (sensor2 == HIGH) && (sensor3 == LOW) && (sensor4 == LOW) && (sensor5 == LOW)) {
            // Estado em que o penultimo sensor a esquerda e o ultimo sensor a esquerda estão detectando a linha preta.
            // Nesse estado queremos que faça uma curva acentuda a direita.
            // 255 representa um duty cycle de 100% no PWM e 64 representa 25% no PWM
            // ou seja o motorA irá rodar com 25 % da sua velocidade e motorB com 100% da sua velocidade maxima.
            analogWrite(motorA, 64);
            analogWrite(motorB, 255);
            v1_star = 64;
            v2_star = 255;
        }
        else {
            // Estado de excessão, pelo problema solucionado por esse seguidor, não devemos ter mais de 2 sensores detectando a linha preta.
            // Alem disso, se dois sensores detectarem a linha preta, esse sensores devem ser vizinhos um do outro.
            // Caso essas regras sejam quebradas, entraremos nesse laço, aqui iremos manter a mesma velocidade nos motores que calculamos no loop anterior.
            analogWrite(motorA, v1_star);
            analogWrite(motorB, v2_star);
        }
    }
}