#include <FalconRobot.h>
#include <SoftwareSerial.h>

SoftwareSerial NewUART(10, 11); //Programando pinos digitais para atuarem como portas de comunicacao serial(10 - RX; 11 - TX)
FalconRobotMotors motors(5, 7, 6, 8); //Portas digitais utilizadas para o comando dos motores
FalconRobotDistanceSensor distanceSensor (2,3); //Portas digitais utilizadas para o comando do sensor Ultrassonico
FalconRobotLineSensor left(A2); //Porta analogica utilizada para o sensor infravermelho da esquerda
FalconRobotLineSensor right(A3); //Porta analogica utilizada para o sensor infravermelho da direita

#define LimiteSup 10 //Distancia maxima vista pelo sensor Ultrassonico(condicao de parada), em cm
#define LimiteInf 5 //Distancia minima vista pelo sensor Ultrassonico(condicao de parada), em cm
#define WhiteLine 700 //Valor intermediario entre uma superficie branca ou preta. Acima desse valor temos um plano escuro
#define PontoInteresse 100//Valor referente ao ponto de interesse 
char buff = " "; //Buffer utilizado para armazenar o caracter de comando via serial
int MotorsPower = 55; //Porcentagem minima de forca necessaria para movimentar os motores
bool ObstDetect = false;
int ValueLeft = 0, LastLeft = 0;
int ValueRight = 0, LastRight = 0;
int count = 0;

void setup() { 
  Serial.begin(115200);
  NewUART.begin(9600); //Inicializando comunicacao serial com o baudrate especificado
}

void loop(){
  buff = NewUART.read(); //Armazenando os dados recebidos via comunicacao serial
  switch(buff){
    case 'f': //Movimentacao para frente(FOWARD)
      while(NewUART.read() != 's'){ //Condicao de parada(STOP)
        if (FindPoint()){ //Verificando limites inferior e superior por meio da funcao DistanciaOK() e analisando se o carrinho esta dentro da faixa permitida mediante a funcao InsideMap()
          Serial.println("S");
          NewUART.write('s');//Chegou no Ponto
          DontTouchMe(); //Freando os motores atraves da funcao DontTouchMe()
          break;
        }else if(DistanciaOK()){
          FollowingLine(); //Movimentando o carrinho para frente
        }else{
          break;
        } 
      }
    break;
    case 'l': //Movimentacao para o lado esquerdo(LEFT)
      Turn90dl();
      ReajustePTurn90dl();
       while(NewUART.read() !='s'){
         if (FindPoint()){ //Verificando limites inferior e superior por meio da funcao DistanciaOK() e analisando se o carrinho esta dentro da faixa permitida mediante a funcao InsideMap()
          Serial.println("S");
          NewUART.write('s');//Chegou no Ponto
          DontTouchMe(); //Freando os motores atraves da funcao DontTouchMe()
          break;
        }else if(DistanciaOK()){
          FollowingLine(); //Movimentando o carrinho para frente
        }else{
          break;
        }
      }
    break;
    case 'r': //Movimentacao para o lado direito(RIGHT)
      Turn90dr();
      ReajustePTurn90dr();
      while(NewUART.read() !='s'){
        if (FindPoint()){ //Verificando limites inferior e superior por meio da funcao DistanciaOK() e analisando se o carrinho esta dentro da faixa permitida mediante a funcao InsideMap()
          Serial.println("S");
          NewUART.write('s');//Chegou no Ponto
          DontTouchMe(); //Freando os motores atraves da funcao DontTouchMe()
          break;
        }else if(DistanciaOK()){
          FollowingLine(); //Movimentando o carrinho para frente
        }else{
          break;
        }
      }
    break;
    case 'L': //Movimentacao default para a posicao inicial
      while(DistanciaOK() && NewUART.read() !='s'){
        if(!FindPoint()){
          FollowingLine();
        }else{
          DontTouchMe();
          break;   
        }
      }
      Turn90dl();
      ReajustePTurn90dl();
    break;
    case 'F': //Movimentacao para frente(Front)
      while(NewUART.read() != 's'){
        if (!DistanciaOK()){ //Verificando limites inferior e superior por meio da funcao DistanciaOK()
          DontTouchMe(); //Freando os motores atraves da funcao DontTouchMe()
          break;
        }else{ 
          motors.leftDrive(MotorsPower, FORWARD);
          motors.rightDrive(MotorsPower, FORWARD);
        }
     }
    break;
    case 't': //Movimentacao para tras(BACKWARD)
      while(NewUART.read() != 's'){ //Condicao de parada(STOP)
        motors.leftDrive(MotorsPower, BACKWARD);
        motors.rightDrive(MotorsPower, BACKWARD); 
      }
    break;
    case 'e': //Movimentacao para o lado esquerdo(LEFT)
      while(NewUART.read() != 's'){ //Condicao de parada(STOP)
        motors.rightDrive(MotorsPower, FORWARD); //Movimentando o carrinho para esquerda
      }
    break;
    case 'd': //Movimentacao para o lado direito(RIGHT)
      while(NewUART.read() != 's'){ //Condicao de parada(STOP)
        motors.leftDrive(MotorsPower, FORWARD); //Movimentando o carrinho para direita
      }
    break;
    default:
      motors.stop(); //Mantendo o carrinho parado
      delay(250);
    break;
  }
}

void DontTouchMe(void){
  motors.stop();
  delay(1000);
}

void FollowingLine(void){
  int leftSpeed = 0;
  int rightSpeed = 0;
  Serial.print("LeftSensor: ");
  Serial.println(left.read());
  Serial.print("RightSensor: ");
  Serial.println(right.read());
  Serial.println();

  ValueRight = right.read();
  ValueLeft = left.read();
   // if the both sensors are on the line, drive forward left and right at the same speed
  if((left.read() > WhiteLine) && (right.read() > WhiteLine)) {
    leftSpeed = MotorsPower;
    rightSpeed = MotorsPower;
  }

  // if the line only is under the right sensor, adjust relative speeds to turn to the right
  else if(right.read() > WhiteLine) {
    Serial.println("Esquerda saiu");
    leftSpeed = MotorsPower + 10;
    rightSpeed = 35;
    count = 1;
  }

  // if the line only is under the left sensor, adjust relative speeds to turn to the left
  else if(left.read() > WhiteLine) {
    Serial.println("Direita saiu");
    leftSpeed = 0;
    rightSpeed = MotorsPower + 10;
    count = 2;
  }

  else{
    if((LastLeft > WhiteLine && LastLeft > ValueLeft) | count == 2){
      leftSpeed = 0;
      rightSpeed = MotorsPower + 10;
    }else if((LastRight > WhiteLine && LastRight > ValueRight) | count == 1){
      leftSpeed = MotorsPower + 10;
      rightSpeed = 35;
    }
  }
  
  LastLeft = ValueLeft;
  LastRight = ValueRight;
  // run motors given the control speeds above
  motors.leftDrive(leftSpeed, FORWARD);
  motors.rightDrive(rightSpeed, FORWARD);
  
  delay(10);  // add a delay to decrease sensitivity.
}

bool DistanciaOK(void){
  int distance = 0;
  distance = distanceSensor.read();
  if(distance <= LimiteSup && distance >= LimiteInf){
    if(!ObstDetect){
      NewUART.write("p");
    }
    ObstDetect = true;
    while(ObstDetect){
      DontTouchMe();
      DistanciaOK();
    }
    return false;
  }else{
    if(ObstDetect){
      Serial.println("C");
      NewUART.write("c");
    }
    ObstDetect = false;
    return true;
  }
}

bool FindPoint(void){
  if((left.read() < PontoInteresse) | (right.read() < PontoInteresse)){ //Modificar depois para analisar fita branca. Por enquanto, analisa-se uma fita preta
    return true;
  }else{
    return false;//Não chegou no ponto
  }
}

void Turn90dl(){
  Serial.print("Turn90dL - LEFTSensor: ");
  Serial.println(left.read());
  Serial.print("Turn90dL - RIGHTSensor: ");
  Serial.println(right.read());
  Serial.println();
  if(left.read()>WhiteLine){
    do{
      motors.rightDrive(MotorsPower+10, FORWARD);
    }while(left.read()>WhiteLine);
    while(left.read()<WhiteLine){
      motors.rightDrive(MotorsPower+10, FORWARD);
    }
  }else {
    while(left.read()<WhiteLine){
      motors.rightDrive(MotorsPower+10, FORWARD);
    }
  }
  motors.stop();
  delay(200);
}

void Turn90dr(){
  Serial.print("Turn90dR - LEFTSensor: ");
  Serial.println(left.read());
  Serial.print("Turn90dR - RIGHTSensor: ");
  Serial.println(right.read());
  Serial.println();
  if(right.read()>WhiteLine){
    do{
      motors.leftDrive(MotorsPower-5, FORWARD);
      Serial.print("Passou do Ponto... LeftSensor: ");
      Serial.println(left.read());
      Serial.println("Passou do Ponto... RightSensor: ");
      Serial.println(right.read());
      Serial.println();
    }while(right.read()>WhiteLine);
    while(right.read()<WhiteLine){
      motors.leftDrive(MotorsPower-5, FORWARD);
    }
  }else {
    while(right.read()<WhiteLine){
      motors.leftDrive(MotorsPower-5, FORWARD);
    }
  }
  motors.stop();
  delay(200);
}

void ReajustePTurn90dl(){
  while(left.read()<WhiteLine){
    motors.leftDrive(MotorsPower-10,FORWARD);
  }
  motors.stop();
  delay(100);
}
  
void ReajustePTurn90dr(){
  while(right.read()<WhiteLine){
    motors.rightDrive(MotorsPower,FORWARD);
  }
  motors.stop();
  delay(100);
}
