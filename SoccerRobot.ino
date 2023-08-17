#include <SPI.h>
#include <Pixy.h>

#define MOTOR_1_ENABLE  1
#define MOTOR_1_INPUT_1 2
#define MOTOR_1_INPUT_2 3

#define MOTOR_2_ENABLE 4
#define MOTOR_2_INPUT_1 5
#define MOTOR_2_INPUT_2 6

#define MOTOR_3_ENABLE 7
#define MOTOR_3_INPUT_1 8
#define MOTOR_3_INPUT_2 9

#define MOTOR_4_ENABLE 10
#define MOTOR_4_INPUT_1 11
#define MOTOR_4_INPUT_2 12

class Motor {
  int enable;
  int input1;
  int input2;
  public:
    Motor(int newEnable, int newInput1, int newInput2)
    {
      enable = newEnable;
      input1 = newInput1;
      input2 = newInput2;

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
      int analog = map(speed, 0, 100, 0, 255);
      digitalWrite(enable, HIGH);
      analogWrite(input1, analog);
      digitalWrite(input2, LOW);
    }

    void reverse(float speed)
    {
      int analog = map(speed, 0, 100, 0, 255);
      digitalWrite(enable, HIGH);
      digitalWrite(input1, LOW);
      analogWrite(input2, analog);
    }
};

Pixy pixy;

Motor Motor1(1, 2, 3);
Motor Motor2(4, 5, 6);
Motor Motor3(7, 8, 9);
Motor Motor4(10, 11, 12);

void forward(int speed) {
  Motor1.forward(speed);
  Motor2.forward(speed);
  Motor3.forward(speed);
  Motor4.forward(speed);
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
  Motor1.forward(speed);
  Motor2.reverse(speed);
  Motor3.forward(speed);
  Motor4.reverse(speed);
}

void left(int speed)
{
  Motor1.reverse(speed);
  Motor2.forward(speed);
  Motor3.reverse(speed);
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
}

void turnToBall(int x)
{
  //Cam 316x208
  if (x == -1)
  {
    right(255);
  } 
  else if (x < 158)
  {
    right(255);
  }
  else if (x > 158)
  {
    left(255);
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

//test

void loop() {

  int x = getBallX();
  Serial.print(String(x) + "  ");

  if (x == -1)
  {
    Serial.println("Cannot find");
    right(255);
    delay(100);
    return(true);
  }
  if (x != 158)
  {
    Serial.println("turning to ");
    turnToBall(x);
    delay(100);
    return(true);
  }
  Serial.println("forward");
  forward(255);

  delay(500);
}
