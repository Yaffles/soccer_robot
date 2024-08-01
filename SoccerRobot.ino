#include <SPI.h>
#include <Pixy.h>

#define MOTOR_1_ENABLE  1
#define MOTOR_1_INPUT_1 2
#define MOTOR_1_INPUT_2 3

#define MOTOR_2_ENABLE 4
#define MOTOR_2_INPUT_1 5
#define MOTOR_2_INPUT_2 6

#define MOTOR_3_ENABLE 7
#define MOTOR_3_INPUT_1 9 
#define MOTOR_3_INPUT_2 8
// motor 3 was wrong direction

#define MOTOR_4_ENABLE 12
#define MOTOR_4_INPUT_1 11
#define MOTOR_4_INPUT_2 10

class Motor {
  int enable;
  int input1;
  int input2;
  int max;
  int constant;
  public:
    Motor(int newEnable, int newInput1, int newInput2, bool special=false)
    {
      enable = newEnable;
      input1 = newInput1;
      input2 = newInput2;
      
      max = 150;
      // if (special) {
      //   max = 60;
      // }

      pinMode(newEnable, OUTPUT);
      pinMode(newInput1, OUTPUT);
      pinMode(newInput2, OUTPUT);
    }
    
    void stop()
    {
      digitalWrite(enable, LOW);
      digitalWrite(input1, LOW);
      digitalWrite(input2, LOW);
    }
    
    void forward(float speed)
    {
      // int analog = map(speed, 0, 100, 0, 255); 
      int analog = map(speed, 0, 100, 0, max); 
      digitalWrite(enable, HIGH);
      analogWrite(input1, analog);
      digitalWrite(input2, LOW);
    }

    void reverse(float speed)
    {
      int analog = map(speed, 0, 100, 0, max);
      digitalWrite(enable, HIGH);
      digitalWrite(input1, LOW);
      analogWrite(input2, analog);
    }
};

Pixy pixy;

Motor Motor1(1, 2, 3);
Motor Motor2(4, 5, 6);
Motor Motor3(7, 9, 8);
Motor Motor4(10, 11, 12, true);

void forward(int speed) {
  Motor1.forward(speed);
  Motor2.forward(speed);
  Motor3.reverse(speed);
  Motor4.reverse(speed);
}

void reverse(int speed)
{
  Motor1.reverse(speed);
  Motor2.reverse(speed);
  Motor3.reverse(speed);
  Motor4.reverse(speed);
}

void right(int speed)
{
  Motor1.reverse(speed);
  Motor2.reverse(speed);
  Motor3.reverse(speed);
  Motor4.reverse(speed);
}

void left(int speed)
{
  Motor1.forward(speed);
  Motor2.forward(speed);
  Motor3.forward(speed);
  Motor4.forward(speed);
}

void stop()
{
  Motor1.stop();
  Motor2.stop();
  Motor3.stop();
  Motor4.stop();
}

void setup() {
  pixy.init();
  Serial.begin(9600);
  Serial.print("hello");

  // forward(30);
}

void turnToBall(int x)
{
  //Cam 316x208
  //TODO find minimum power of motor
  if (x > 158)
  {
    int distance = x-158;
    int power = 0;
    if (distance > 100) {
      power = 100;
    }
    else {
      power = distance;
    }
    right(power);
    Serial.println(" right, power is "+ String(power));
  }
  else if (x < 158)
  {
    int distance = 158-x;
    int power = 0;
    if (distance > 100) {
      power = 100;
    }
    else {
      power = distance;
    }
    left(power);
    Serial.println(" left, power is "+ String(power));
  }
}

int getBallX()
{
  uint16_t blocks = pixy.getBlocks();
  if (blocks) {
    for (int i = 0; i < blocks; i++)
    {
      if(pixy.blocks[i].signature == 1)  
      {
        return(pixy.blocks[i].x);
      }
    }
  }
  return(-1);
  
}



void loop() {
  int x = getBallX();
  Serial.print(String(x) + "  ");

  if (x == -1)
  {
    Serial.println("Cannot find");
    right(100);
    delay(100);
    return(true);
  }
  if (abs(158-x) > 10)
  {
    Serial.print("turning to ");
    turnToBall(x);
    delay(100);
    return(true);
  }
  Serial.println("forward");
  forward(100);

  delay(500);


  // Serial.println("Motor 1");
  // Motor1.forward(100);
  // delay(5000);
  // stop();

  // Serial.println("Motor 2");
  // Motor2.forward(100);
  // delay(5000);
  // stop();

  // Serial.println("Motor 3");
  // Motor3.forward(100);
  // delay(5000);
  // stop();

  // Serial.println("Motor 4");
  // Motor4.forward(100);
  // delay(5000);
  // stop();

  // forward(100);
  // forward(100);
}
