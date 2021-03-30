#include "CubeCell_NeoPixel.h"
CubeCell_NeoPixel pixels(1, RGB, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW); //SET POWER
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}
uint8_t i=0;

void loop() {
  // put your main code here, to run repeatedly:

    pixels.setPixelColor(0, pixels.Color(i, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop

    pixels.setPixelColor(0, pixels.Color(0, i, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.


    delay(200); // Pause before next pass through loop

    pixels.setPixelColor(0, pixels.Color(0, 0, i));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop

    i+=10;

}
