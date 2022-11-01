#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ASR_Arduino.h>
#include <CyLib.h>



#define CYCLES_800_T0H  19 // 0.4us
#define CYCLES_800_T1H  38// 0.8us
#define CYCLES_800      60 // 1.25us per bit
#define CYCLES_400_T0H  24// 0.5uS
#define CYCLES_400_T1H  30// 1.2us
#define CYCLES_400      75 // 2.5us per bit

void ASR_NeoPixelShow(
 uint8_t pin, uint8_t *pixels, uint32_t numBytes, bool is800KHz) {

  uint8_t *p, *end, pix, mask;
  uint32_t t, time0, time1, period, startTime, pinMask,saveLoad,pinHigh,pinLow;

  uint8 port=pin / PIN_NUMBER_IN_PORT;
  uint8 PIN_SHIFT=pin % PIN_NUMBER_IN_PORT;
  uint32 REG_DR=CYREG_GPIO_PRT0_DR + port * PORT_REG_SHFIT;
  uint8 Pin_MASK=0x01<<PIN_SHIFT;
  uint8 drVal = (uint8)((* (reg32 *) REG_DR) & (~Pin_MASK));

  pinHigh=drVal|Pin_MASK;
  pinLow=drVal & (~(uint32_t)Pin_MASK);

  p         =  pixels;
  end       =  p + numBytes;
  pix       = *p++;
  mask      = 0x80;
  startTime = 0;

  period = CYCLES_800;
  time0  = period - CYCLES_800_T0H;
  time1  = period - CYCLES_800_T1H;

  saveLoad = CY_SYS_SYST_RVR_REG;
  CY_SYS_SYST_RVR_REG = period;
  CY_SYS_SYST_CVR_REG = 0u;
  //pinMode(pin,OUTPUT);

	for(t=time0;;t=time0) {
		while(CY_SYS_SYST_CVR_REG < period);//wait for bit start
	    (* (reg32 *) REG_DR)=pinHigh; //write high
		if(pix & mask) t = time1;
		
		while(CY_SYS_SYST_CVR_REG > t); // Wait high duration
	    (* (reg32 *) REG_DR)=pinLow;//write low

	    if(!(mask >>= 1)) {                                   // Next bit/byte
	      if(p >= end) break;
	      pix  = *p++;
	      mask = 0x80;
	    }
	}	
  while(CY_SYS_SYST_CVR_REG < period); //wait for the last bit.
 // (* (reg32 *) REG_DR)=pinLow;
  CY_SYS_SYST_RVR_REG = saveLoad; 
  
}




