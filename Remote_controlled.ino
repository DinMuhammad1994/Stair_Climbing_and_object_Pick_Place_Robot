//Remote Controlled Code for Stair Climbing Rover and Bomb Disposable

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>   //header files for lcd

// header files for nrf
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"12345", "56789"};

struct package
{
  int trs;    // value that we are sending to robot
};
typedef struct package Package;
Package data;

struct package1
{
  int alert;// alert are that values that we are taking from our robot
  int temp;
};
typedef struct package1 Package1;
Package1 data1;



int t;
int Alert_State = 0;
int buzz = 2;
int buzzGND = 5;



void setup() {
  Serial.begin(9600);
  lcd.begin();
//  lcd.init();
 Serial.print("Code start");
  // Turn on the blacklight and print a message.
  lcd.backlight();  // to trun on lcd light
  lcd.clear();      // to clear lcd 
  lcd.setCursor(0,0);  
  lcd.print(" INDUS  UNIVERSITY ");
  lcd.setCursor(0,1);
  lcd.print("  BE   ELECTRICAL  ");
  lcd.setCursor(0,2);
  lcd.print("    BATCH  16-A    ");
  
  delay(5000); // 5 second delay
  
  lcd.clear();      // to clear lcd 
  lcd.setCursor(0,0);  
  lcd.print(" SUPERVISOR ");
  lcd.setCursor(0,1);
  lcd.print(" ENGR.MOHSIN SHAIKH");
  delay(5000); // 5 second delay
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("M.KASHIF (300-2016)");
  lcd.setCursor(0,1);
  lcd.print("ATIF     (048-2016)");
  lcd.setCursor(0,2);
  lcd.print("AQIB     (040-2016)");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("<<REMOTE ACTIVATED>>");


  pinMode(buzz,OUTPUT);
  pinMode(buzzGND,OUTPUT);
  digitalWrite(buzz, LOW);
  digitalWrite(buzz, HIGH);
  delay(500);
  digitalWrite(buzz, LOW);
  delay(500);
  digitalWrite(buzz, HIGH);
  delay(300);
  digitalWrite(buzz, LOW);
  delay(300); 
  
  radio.begin();
  radio.setAutoAck(false);
  radio.openWritingPipe(addresses[1]);      // 00002
  radio.openReadingPipe(1, addresses[0]);   // 00001
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("<<REMOTE ACTIVATED>>");
}
void loop() {
  delay(500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEMP:");
  lcd.setCursor(7,0);
  lcd.print("*C ALERT:");
  
  lcd.setCursor(0,1);
  lcd.print("ROBOT : ");
  lcd.setCursor(0,2);
  lcd.print("ARM   : ");
  lcd.setCursor(0,3);
  lcd.print("JAW   : ");
  
  delay(5);
  radio.stopListening();   // to stop listning

  int x1 = analogRead(A0);
  int y1 = analogRead(A1);
  int x2 = analogRead(A2);
  int y2 = analogRead(A3);
  delay(5);


  if (x1 > 1000) {
    data.trs = 1;     // assgining value to trs 
    radio.write(&data, sizeof(data)); // syntax to send data
    Serial.print(data.trs);
    delay(100);
    Serial.println("FORWARD");
    lcd.setCursor(10,1);
    lcd.print("FORWARD  ");
  }
  else if (x1 < 100) {
    data.trs = 2;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("REVERSED");
    lcd.setCursor(10,1);
    lcd.print("REVERSE  ");
  
  }
  else if (y1 < 100) {
    data.trs =  3;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("LEFT TRUN");
    lcd.setCursor(10,1);
    lcd.print("TURN-LEFT");
  
  }
  else if (y1 > 1000) {
    data.trs =  4;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("RIGHT TRUN");
    lcd.setCursor(10,1);
    lcd.print("TURN-RIGHT");
  }


  if (x2 > 1000) {
    data.trs =  5;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("Robot Arm Upward");
    lcd.setCursor(10,2);
    lcd.print("UPWARD   ");
    
  }
  else if (x2 < 100) {
    data.trs =  6;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("Robot Arm Downward");
    lcd.setCursor(10,2);
    lcd.print("DOWNWARD ");
    
  }
  else if (y2 < 100) {
    data.trs =  7;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("Jaw Close");
    lcd.setCursor(10,3);
    lcd.print("CLOSE    ");
  }
  else if (y2 > 1000) {
    data.trs =  8;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("JAW Open");
    lcd.setCursor(10,3);
    lcd.print("OPEN    ");
  }
  else if (x1 > 500 && x1 < 700 && y1 > 480 && y1 < 700)
  {
    data.trs = 9;
    radio.write(&data, sizeof(data));
    Serial.print(data.trs);
    delay(100);
    Serial.println("ROBOT Stop");
    lcd.setCursor(10,1);
    lcd.print("STOP     ");

    lcd.setCursor(10,2);
    lcd.print("CENTRE   ");

    lcd.setCursor(10,3);
    lcd.print("CENTRE");   
  }
  Serial.println(" ");

   delay(500);
  
 
  delay(5);
  radio.startListening();
  if ( radio.available())
  {
    while (radio.available())
    {
      radio.read( &data1, sizeof(data1) );  // data of DHT11
 }

    t = data1.temp;
    Serial.println("Rcv Robot data:");
    Serial.print("Temperature");
    Serial.println(t);
    lcd.setCursor(5,0);
    lcd.print(t);
    
    int Alert = data1.alert;
    Serial.print("Alert : ");
    Serial.println(Alert);
   
    

    if (Alert == 0)
    {
      Serial.println("No any alert detected");
      lcd.setCursor(17,0);
      lcd.print("NO");
    }

    if (Alert == 1)
    {
      Serial.println("Smoke Detected");
      lcd.setCursor(17,0);
      lcd.print("S");
    
      Buz();
      Alert == 0;
    }

    if (Alert == 2)
    {
      Serial.println("Flame Detected");
      lcd.setCursor(17,0);
      lcd.print("F");
      Buz();
      Alert == 0;
    }
  }
}


void Buz()
{
  digitalWrite(buzzGND, LOW);
  digitalWrite(buzz, HIGH);
  delay(500);
  digitalWrite(buzz, LOW);
  delay(500);
  digitalWrite(buzz, HIGH);
  delay(500);
  digitalWrite(buzz, LOW);
  delay(500);
}
