#ifndef __asr6601__
#error "only asr6601 series has DAC."
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin();
  
  /* GPIO9 is DAC output pin
   * dacWrite(uint16_t val);
   * \param [IN] val    dac output val in mV;
   */ 
  dacWrite(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
}