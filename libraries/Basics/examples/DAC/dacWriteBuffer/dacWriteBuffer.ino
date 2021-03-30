#ifndef __asr6601__
#error "only asr6601 series has DAC."
#endif

#define BUFF_SIZE 1024

uint16_t dac_buf[BUFF_SIZE];

void setup() {
  // put your setup code here, to run once:
  Serial.begin();
  float pi = 3.14;
  float um = 1.5;
  uint16_t size = BUFF_SIZE;
  for(int i=0; i<size; i++){
      dac_buf[i] = (uint16_t)((um*sin((1.0*i/(size))*2*pi)+1.65)*4095/3.3);
  }
  
  /* GPIO9 is DAC output pin
   * dacWriteBuffer(uint16_t* buff,uint16_t size,bool repeat);
   * \data value 0-4095 for 0-3.3V, each data continues 1us
   * \param [IN] buff    dac buffer pointer
   * \param [IN] size    dac buffer size, max size is 1024
   * \param [IN] repeat  true: dac out repeatly; false: dac out once.
   * if repeat is false, when dac done, gpio 9 keeps the last value of output data.
   */
  dacWriteBuffer(dac_buf,size,1);
}

void loop() {
  // put your main code here, to run repeatedly:
}