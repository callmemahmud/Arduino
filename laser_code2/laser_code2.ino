#include <SoftwareSerial.h>


//this is not gsm module phone number


SoftwareSerial mySerial(2, 3);

#define LASER_RECEIVER_PIN 4
#define LASER_TRANSMIT_PIN 5
//fire_flag = 0 means tripwire detected
boolean laser_flag = 0;

#define BUZZER_PIN 8

void setup()
{
  //----------------------------------------------------------------
  //Begin serial communication: Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  //----------------------------------------------------------------
  //Begin serial communication: SIM800L
  mySerial.begin(9600);
  //----------------------------------------------------------------
  pinMode(LASER_RECEIVER_PIN,INPUT);
  pinMode(LASER_TRANSMIT_PIN,OUTPUT);
  //by default the LASER Transmitter is ON
  digitalWrite(LASER_TRANSMIT_PIN,HIGH);
  //----------------------------------------------------------------
  pinMode(BUZZER_PIN, OUTPUT);
  //by default the BUZZER is OFF
  digitalWrite(BUZZER_PIN,LOW);
  //----------------------------------------------------------------
  Serial.println("Initializing...");
   delay(1000);
  //Once the handshake test is successful, it will back to OK
  //----------------------------------------------------------------
}

void loop()
{
  while(Serial.available())
  {
 mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }

  
  int laser_value = digitalRead(LASER_RECEIVER_PIN);
  
  
  //The laser trip wire is detected, trigger Alarm and send sms
  if(laser_value == LOW) {
    digitalWrite(BUZZER_PIN,HIGH);
    //----------------------------------------------------------------
    if(laser_flag == 0) {
      Serial.println("Laser Tripwire Detected.");
      laser_flag == 1;
      make_call();
    }
   
  }
  
  //No laser tripwire is detected, turn OFF Alarm
  else {
    //Serial.println("Laser Tripwire !Detected.");
    digitalWrite(BUZZER_PIN,LOW);
    laser_flag = 0;
  }
  
}



void make_call()
{   mySerial.println("AT");
    delay(1000);
    Serial.println("calling....");
    mySerial.println("ATD+ +8801552482894;");
    delay(20000); //20 sec delay
    mySerial.println("ATH");
    delay(1000); //1 sec delay
}
