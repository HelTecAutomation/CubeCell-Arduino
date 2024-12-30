/*
  SD card read/write

  SD card reader attached to SPI1 bus as follows:
 ** MOSI1 - GPIO1
 ** MISO1 - GPIO2
 ** CLK1  - GPIO3
 ** CS1   - GPIO4
*/

#include <SPI.h>
#include <SD.h>

#define CS1 GPIO4

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  SPI1.begin(SCK1, MISO1, MOSI1, CS1);

  Serial.print("Initializing SD card...");

  if (!SD.begin(CS1)) {
    Serial.println("failed!");
    while (1);
  }
  Serial.println("done");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("Testing 1, 2, 3...");
    // close the file:
    myFile.close();
    Serial.println("done");
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening test.txt to write!");
  }
  delay(1000);
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("Error opening test.txt to read!");
  }
}

void loop() {
  // nothing happens after setup
}
