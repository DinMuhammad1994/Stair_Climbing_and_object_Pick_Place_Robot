#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
#include <Servo.h>

Servo servoarm;  // create servo object to control a servo
Servo servojaw;

int angle = 25;   // initial angle  for servo
int angleStep = 15;

int jawpos_close = 55;  // close at initial stage
int jawpos_open = 0;
const byte addresses[][6] = {"12345", "56789"};

// trans package
struct package
{
  int trs;
};

typedef struct package Package;
Package data;


struct package1
{
  int alert;
  int temp;
};
typedef struct package1 Package1;
Package1 data1;



// Temperature and humidity sensor
#include "DHT.h"
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// motor LEFT
int in1 = 2;
int in2 = 3;
// motor RIGHT
int in3 = 5;
int in4 = 4;

int Smoke = 0;
int gas_sensor = A0;
int Smoke_sp = 950;

int Flame = 0;
int Flame_sensor = A1;
int flame_Sp = 500;

int buzzer = 4;




void setup() {
  Serial.begin(9600);
  servoarm.attach(9);
  servojaw.attach(10);
  servoarm.write(angle);
  Serial.print("init");
  dht.begin();
  radio.begin();
  radio.setAutoAck(false);
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_250KBPS);
  radio.startListening();

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  servojaw.write(jawpos_close);              // tell servo to go to position in variable 'pos'
  delay(15);                       // waits 15ms for the servo to reach the position


}

void loop() {

  Smoke = analogRead(gas_sensor);
  Flame = analogRead(Flame_sensor);
//  Serial.print("MQ-2 gas value:"); Serial.println(Smoke);
//  Serial.print("Flame value:");    Serial.println(Flame);

//  Serial.println("********");
  
  delay(5);   
  radio.startListening();
 
  
  if ( radio.available())
  {
    while (radio.available())
    {
      radio.read( &data, sizeof(data) );
    }

    int a = data.trs;
    Serial.print("Recieving signal:");
    Serial.print(a);
    Serial.println("");
    delay(100);
    if (a == 1)
    {
      Forward();
    }

    if (a == 2)
    {
      Reverse();
    }

    if (a == 3)
    {
      left();
    }

    if (a == 4)
    {
      right();
    }
    if (a == 5)
    {
      if (angle > 25 && angle <= 90) {
        angle = angle - angleStep;
        if (angle < 25) {
          angle = 25;
        } else {
          servoarm.write(angle); // move the servo to desired angle
        }
      }
      delay(100);
      Serial.println("Robot Arm Forward");
    }

    if (a == 6)
    {
      if (angle >= 25 && angle <= 90) {
        angle = angle + angleStep;
        if (angle > 90)
        {
          angle = 90;
        }
        else {
          servoarm.write(angle); // move the servo to desired angle
        }
      }
      delay(100);
      Serial.println("Robot Arm backward");
    }
    if (a == 7)
    {
      servojaw.write(jawpos_close);
      delay(15);                       // waits 15ms for the servo to reach the position

      Serial.println("Jaw Close");
    }
    if (a == 8)
    {
      servojaw.write(jawpos_open);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position

      Serial.println("JAW Open");
    }
    if (a == 9)
    {
      Stop();
    }
  }

  delay(500);
  
  delay(5);
  radio.stopListening();
  {

    int t = dht.readTemperature();
 //   Serial.print("Temperature: ");
 //   Serial.print(t);
 //   Serial.println(" *C ");
    
    data1.temp = t;
    radio.write(&data1, sizeof(data1));
    delay(100);
  }




//  if (Smoke > Smoke_sp) {
//    data1.alert = 1;
//    radio.write(&data1, sizeof(data1));
//        Serial.print(data1.alert);
//        delay(100);
//    Serial.println("Smoke detected");
    
//  }
//  else
  if (Flame < flame_Sp) {
    data1.alert = 2;
    radio.write(&data1, sizeof(data1));
    Serial.print(data1.alert);
    delay(100);
    Serial.println("Flame detected");
  }
  else
  {
    data1.alert = 0;
    Serial.print("NO ALERT: ");
    radio.write(&data1, sizeof(data1));
    Serial.println(data1.alert);
    delay(100);
  }


}




void Forward()
{
  Serial.println("Forward");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

}


void Reverse()
{
  Serial.println("Reverse");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

}

void left()
{
  Serial.println("Left");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);


}
void right()
{ Serial.println("Right");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

}

void Stop()

{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("MotorStop");
}
