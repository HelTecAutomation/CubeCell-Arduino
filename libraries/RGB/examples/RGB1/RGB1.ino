#include "CubeCell_NeoPixel.h"
CubeCell_NeoPixel pixels(1, RGB, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW); //SET POWER
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}
uint8_t i=50;

void loop() {
  pixels.clear(); 
  delay(500);

  pixels.setPixelColor(0, pixels.Color(i, 0, 0));
  pixels.show();
  delay(500); 

  pixels.setPixelColor(0, pixels.Color(0, i, 0));
  pixels.show();  
  delay(500);

  pixels.setPixelColor(0, pixels.Color(0, 0, i));
  pixels.show();   
  delay(500); 
}
