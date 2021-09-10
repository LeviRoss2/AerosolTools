#include <Wire.h>
#include "SDP6x_W0.h"
#include "SDP6x_W1.h"
#include "SDP6x_W2.h"
#include "SD.h"
#include <SPI.h>

#define USB true

unsigned long Time = millis();
const unsigned long  iterTime = 1000;
unsigned long  last = 0;
const unsigned long  readIter = 33;
unsigned long  lastRead = 0;

float difPressure0;
float difPressure1;
float difPressure2;

int led = LED_BUILTIN;

File file;  // Define file variable used to save data into the filename
const int chipSelect = BUILTIN_SDCARD;
char filename[12]; // Stores the name of the resulting log file


//------------------------------------------------------------------------------
void setup(void) {

  if(Serial){
    Serial.begin(115200);
  }

  delay(1000);

  Wire.begin();
  Wire1.begin();
  Wire2.begin();
  Serial.begin(9600);
  difPressure0 = 0.0;
  difPressure1 = 0.0;
  difPressure2 = 0.0;

  pinMode(led, OUTPUT); //LED for reference

  delay(1000);

  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    while (1) {
      //      Serial.println("Teensy 3.6 SD fail. Reset card and try again.");
      digitalWrite(led, !digitalRead(led));
      delay(2000);
    }
  }

  int n = 0;
  snprintf(filename, sizeof(filename), "LOG%03d.csv", n); // includes a three-digit sequence number in the file name
  while (SD.exists(filename)) {
    n++;
    snprintf(filename, sizeof(filename), "LOG%03d.csv", n);
  }
  if (Serial) {
    Serial.println(n);
    Serial.println(filename);
  }

  if (Serial) {
    Serial.println("File created successfully");
    if (USB == false){
      Serial.println("Serial data output disabled. Enable USB in code to see data live.");
    }
    else{
      Serial.println("Serial data output enabled.");
    }
  }

  file.close();

  delay(50);

  file = SD.open(filename, O_CREAT | O_WRITE);
  file.println("Teensy Time (ms), Diff Press 1 (Pa), Diff Press 2 (Pa), Diff Press 3 (Pa)");
  file.close();

  file = SD.open(filename, O_CREAT | O_WRITE);

}

void loop() {
  Time = millis();

  if ((Time - last) >= iterTime) {
    last = Time;
    file.flush();
  }

  if ((Time - lastRead) >= readIter) {

    lastRead = Time;
    digitalWrite(led, !digitalRead(led));
    difPressure0 = SDP6x0.GetPressureDiff0();
    difPressure1 = SDP6x1.GetPressureDiff1();
    difPressure2 = SDP6x2.GetPressureDiff2();
    Time = millis();

#if USB==true
    Serial.print(Time);
    Serial.print("      ");
    Serial.print(difPressure0);
    Serial.print(",  ");
    Serial.print(difPressure1);
    Serial.print(",  ");
    Serial.print(difPressure2);
    Serial.println();
#endif //USB == true

    char buf[25];
    sprintf(buf, "%lu,%f,%f,%f", Time, difPressure0, difPressure1, difPressure2);
    file.println(buf);
  }
}
