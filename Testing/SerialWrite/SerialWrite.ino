
// zoomkat 10-22-11 serial servo test
// type servo position 0 to 180 in serial monitor
// or for writeMicroseconds, use a value like 1500
// for IDE 0022 and later
// Powering a servo from the arduino usually *DOES NOT WORK*.

String readString;
#include <Servo.h> 
//Servo myservo;  // create servo object to control a servo 

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
   // Serial.println(readString + " MPH");  
    int n = readString.toInt();  //convert readString into a number

    // write data to file

    // show data in serial monitor


    //empty for next input
    readString=""; 
  } 

  

}



