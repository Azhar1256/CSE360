#include "TinyGPS++.h"
#include "SoftwareSerial.h"

String textForSMS;

SoftwareSerial serial_connection(10,11); //tx,rx 
TinyGPSPlus gps;// GPS object to process the NMEA data
double longitude; 
double latitude;
String datareal;

int pbtn = 12; //Button Connnected
bool pressed = 0; //if pressed we will get a low signal so 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);            //This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600); //This opens up communications to the GPS
  Serial.println("GPS Start");   //To show in the serial monitor that the sketch has started

  pinMode(pbtn, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(serial_connection.available())              //While there are incoming characters  from the GPS
  {
    gps.encode(serial_connection.read());           //This feeds the serial NMEA data into the library one char at a time
  }
  bool currentState = digitalRead(pbtn);
  if (currentState == pressed)          //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println(gps.loacation.lat(),6);
    Serial.println(gps.loacation.lng(),6);

    while(digitalRead(pbtn) == pressed){
        //We will do nothing as it is already pressed
    }
    }
}
