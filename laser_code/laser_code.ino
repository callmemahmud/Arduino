#include <SoftwareSerial.h>   // library of serial conncetion 


//this is not gsm module phone number
const String PHONE = "+8801552482894";     // phone number 

#define rxPin 3     // arduino 3 pin
#define txPin 2
SoftwareSerial sim800L(rxPin,txPin);

#define LASER_RECEIVER_PIN 4    // arduino 4 photo-transistor
#define LASER_TRANSMIT_PIN 5      // arduino laser
//fire_flag = 0 means tripwire detected
boolean laser_flag = 0;

#define BUZZER_PIN 8

void setup()
{
  //----------------------------------------------------------------
  //Begin serial communication: Arduino IDE (Serial Monitor)
  Serial.begin(9600);      // seral conncetion was TOO HIGH (bit per sec.)  
  
  //Begin serial communication: SIM800L
  sim800L.begin(9600);    // sim module's serial conncetion UPDATED
  
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

  //Once the handshake test is successful, it will back to OK
  Serial.println("AT");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(1000);
  //----------------------------------------------------------------
}

void loop()
{
  while(sim800L.available()){
  Serial.println(sim800L.readString());
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

void make_call()                   // modifid this fuction 
{
    Serial.println("calling....");       
    sim800L.println("ATD"+PHONE+";");
    delay(20000); //20 sec delay
    sim800L.println("ATH");
    delay(1000); //1 sec delay
}
