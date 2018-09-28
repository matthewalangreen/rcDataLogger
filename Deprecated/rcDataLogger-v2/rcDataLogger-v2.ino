

/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
String fileName;



// Power by connecting Vin to 3-5V, GND to GND
// Uses I2C - connect SCL to the SCL pin, SDA to SDA pin
// See the Wire tutorial for pinouts for each Arduino
// http://arduino.cc/en/reference/wire
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

const int chipSelect = 4;

void setup() {
  //randomSeed(analogRead(0));
  //randomer = String(random(1,100));
  //fileName = "rc_" + randomer + ".txt";
  fileName = "rc.txt";
  
  pinMode(13, OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  if(Serial) {
    Serial.print("Initializing SD card...");
  }
  

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    if(Serial) {
      Serial.println("Card failed, or not present");
    }
    
    // don't do anything more:
    return;
  }
  if(Serial) {
    Serial.println("card initialized.");
    Serial.println("Adafruit_MPL3115A2 test!");
  }
  

  File dataFile = SD.open(fileName, FILE_WRITE);
  String header = "Pascals--Inches (Hg)--,Altitude--meters--,Temperature--*C--,Millis()";
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(header);
    dataFile.close();
    // print to the serial port too:
    Serial.println(header);
  }
}

void loop() {
  
  
  if (! baro.begin()) {
    if(Serial) {
       Serial.println("Couldnt find sensor");
       Serial.println("Starting time in seconds: " + millis()/1000);
    }
     
      return;
    }

      // make a string for assembling the data to log:
     String dataString = "";

     float pascals = baro.getPressure();
  // Our weather page presents pressure in Inches (Hg)
  // Use http://www.onlineconversion.com/pressure.htm for other units
 // Serial.print(pascals/3377); Serial.println(" Inches (Hg)");

  float altm = baro.getAltitude();
  //Serial.print(altm); Serial.println(" meters");

  float tempC = baro.getTemperature();
  //Serial.print(tempC); Serial.println("*C");

 // delay(50);
 


  // read three sensors and append to the string:
//  for (int analogPin = 0; analogPin < 3; analogPin++) {
//    int sensor = analogRead(analogPin);
//    dataString += String(sensor);
//    if (analogPin < 2) {
//      dataString += ",";
//    }
//  }

  // put data in string formatted, "pressure,altitude,temperature"
  dataString += String(pascals/3377);
  dataString += ",";
  dataString += String(altm);
  dataString += ",";
  dataString += String(tempC);
  dataString += ",";
  dataString += String(millis()/1000);


  digitalWrite(13, HIGH);
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  
  
  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    if(Serial) {
       Serial.println(dataString);
    }
    digitalWrite(13, LOW);
  }
  // if the file isn't open, pop up an error:
  else {
    if (Serial) {
      Serial.println("error opening datalog.txt");
    }
    
  }
}











