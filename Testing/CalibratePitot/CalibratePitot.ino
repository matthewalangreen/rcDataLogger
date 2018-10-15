#include <SPI.h>
#include <SD.h>
#include <Wire.h>

String readString;
String fileName;
const int chipSelect = 4;
String dataString = "";
String consoleString = "";
float calculatedMPH = 0;

void setup() 
{
  fileName = "CALIBR8.txt";
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  
  if(Serial) 
  {
    Serial.print("Initializing SD card...");
  }
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
  {
    if(Serial)  { Serial.println("Card failed, or not present"); }
    return; // don't do anything more:
  }
  
  if(Serial) { Serial.println("card initialized."); }

  File dataFile = SD.open(fileName, FILE_WRITE);
  String tabCharacter = "\t";
  String header = "Pressure,Calculated MPH, Input MPH";
  
  // if the file is available, write to it:
  if (dataFile) 
  {
    dataFile.println(tabCharacter);
    dataFile.println(header);
    dataFile.close();
    // print to the serial port too:
    if(Serial) { Serial.println(header); }
  }
}

void loop() {

    if(Serial.available()) 
    {
      // get value, wait, get another value
      char c = Serial.read();  
      readString += c; 
      delay(2); 
      c = Serial.read();
      readString += c;
    }

    // initialize dataString for use
    dataString = "";

    // put data in string formatted, "airspeed"
    // Vout = VS × (0.2 × P(kPa)+0.5) ± 6.25% VFSS
    float reading = analogRead(0);
    float pressure = map(reading,0,1023,0,5); // not sure that I need to do this mapping...
    int VS = 5; // input voltage
    
    float vOut = VS *(0.2*reading+0.5);
    calculatedMPH = 0.32*vOut - 280.943 -6;  // inverse of our function: https://www.desmos.com/calculator/aze1yror77
    dataString += vOut;
    dataString += ",";
    dataString += String(calculatedMPH);

    // flicker onboard LED to show data collected
    digitalWrite(13, HIGH);

    if (readString.length() >0) 
    {
      consoleString = readString;
      //empty for next input
      readString=""; 
    } 



    // add value collected from serial monitor to data string
      dataString += ",";
      dataString += consoleString;
      
      // write data to file
      File dataFile = SD.open(fileName, FILE_WRITE);
      
      // if the file is available, write to it:
      if (dataFile) 
      {
        dataFile.println(dataString);
        dataFile.close();
        
        // print to the serial port too:
        if(Serial) 
        {
          Serial.println(dataString);
        }
        
        digitalWrite(13, LOW);
        delay(200);
      }
      // if the file isn't open, pop up an error:
      else 
      {
        if (Serial) 
        {
          Serial.println("error opening datalog.txt");
        }
      }
}



