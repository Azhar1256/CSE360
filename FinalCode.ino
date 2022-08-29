#include "TinyGPS++.h"
#include <SoftwareSerial.h>

//Declaring the pins for our GSM Module
SoftwareSerial SIM900A(9,8);
String textForSMS;

//Pins for our Sonar Sensor and Buzzer
int buzz = 10; //Buzzer Connected
int echo = 11; //Echo Connected
int trig = 12; //Trig Connected
long duration;
float distance;

//tx and rx pins for our GPS module
SoftwareSerial serial_connection(2,3); //tx,rx 
TinyGPSPlus gps;// GPS object to process the NMEA data
double longitude; 
double latitude;
String datareal;

//Connecting our Moisture Sensor (Analogue)
int moisture = A5; // moisture sensor is connected with the analog pin A5 of the Arduino
int msvalue = 0; // moisture sensor value

//Push Button
int pbtn = 13; //Button Connnected
bool pressed = 0; //if pressed we will get a low signal so 0

void setup() {
  // put your setup code here, to run once:

  //Seetting the serial baudrate
  Serial.begin(9600);

  //Setting, Sonar Sensors Trigger and Echo Pins
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  //Setting Moisture Sensors pin
  pinMode(moisture, INPUT);
  Serial.println("Moisture Sensor Connected");

  //Used built in pull-up resistor of Arduino for the push button
  pinMode(pbtn, INPUT_PULLUP);
  Serial.println("Button Connected");

  //GSM Module
  SIM900A.begin(9600);   // GSM Module Baud rate - communication speed
  Serial.println ("Text Messege Module Ready & Verified");
  delay(100);

}

void loop() {
  // put your main code here, to run repeatedly:

  //Trigger pin sending sound waves
  digitalWrite(trig,HIGH);
  delayMicroseconds(100);
  digitalWrite(trig,LOW); //Stopped sending sound waves

  //Calculating the time to receive the wave
  duration=pulseIn(echo,HIGH);

  //Calculating the distance, speed = speed of sound in air and
  //dividing by 2 because the time to travel and return was taken into account
  distance=duration*.034/2; //Calculating the distance

  //If our distance is less than 20cm the buzzer will get activated
  if(distance<20){
    Serial.println("BUZZZ!!!");
    Serial.println(distance);
    play_buzz_distance();
  }

  //Getting the moisture value using our sensor
  msvalue = analogRead(moisture);

  //If msvalue <= 700 buzzer will activated
  //less msvalue means, more water or moisture
  if (msvalue <= 700){
    play_buzz_water();
    delay(100);  
  }

  //Storing the current state whether the button is pressed or not
  bool currentState = digitalRead(pbtn); 
  //If the button is pressed then we will send a message
  if (currentState == pressed){

    //Storing the value of longitude and latitude
    latitude = gps.location.lat(), 6 ;
    longitude = gps.location.lng(), 6 ;

    //Formating the data
    long datareal = int(latitude);
    int dp1 = ( latitude - datareal) * 100000;

    long datareal2 = int(longitude);
    int dp2 = (longitude - datareal2 ) * 100000;

    //Formating the message that we want to sent
    textForSMS = "Longitude:  ";
    textForSMS.concat(longitude);
   
    textForSMS = textForSMS + " Latitude: ";
    textForSMS.concat(latitude);
   
    textForSMS = textForSMS + "Please Help Me!!!";
    //Finally calling our SendMessage function whice take the message as parameter and sends it to the user
    SendMessage(textForSMS);
    //After the message is send succesfully the buzzer will get activated and stop after some times
    tone(buzz,0);
    delay(2000);
    noTone(buzz);
    //This loop actually works like a key debouncing technique
    while(digitalRead(pbtn) == pressed){
      //We will do nothing as it is already pressed
    }
  }

}

//Function for playing buzzer if distance is less than 20cm
void play_buzz_distance(){
  tone(buzz, 100);
  delay(100);
  noTone (buzz);
  delay(100);
  tone(buzz, 100);
  delay(100);
  noTone (buzz);
  delay(100);
}

//Function for playing buzzer if moisture is less than 700
void play_buzz_water(){
  tone(buzz, 400);
  delay(500);
  noTone (buzz);
  delay(500);
}

////Function for sending message to a specific user
void SendMessage(String message){
  Serial.println ("Sending Message please wait....");
  //Text Mode initialisation
  SIM900A.println("AT+CMGF=1");    
  delay(1000);
  Serial.println ("Set SMS Number");
  // Receiver's Mobile Number
  SIM900A.println("AT+CMGS=\"+8801796063882\"\r"); 
  delay(1000);
  Serial.println ("Set SMS Content");
  // Messsage content passed in parameter
  SIM900A.println(message);
  delay(100);
  Serial.println ("Done");
  SIM900A.println((char)26);//   delay(1000);
  Serial.println ("Message sent succesfully");
}
