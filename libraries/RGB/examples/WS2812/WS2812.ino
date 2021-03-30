#include "CubeCell_NeoPixel.h"
CubeCell_NeoPixel pixels(16, GPIO7, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW); //SET POWER
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}
uint8_t i=0, j=0;

void loop() {
    pixels.setPixelColor(j, pixels.Color(i, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop

    pixels.setPixelColor(j, pixels.Color(0, i, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop

    pixels.setPixelColor(j, pixels.Color(0, 0, i));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(200); // Pause before next pass through loop
    pixels.setPixelColor(j, pixels.Color(0, 0, 0));
    
    i+=10;
    j++;
    if(j>15)
      j=0;

}
