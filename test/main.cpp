
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "cubecell_SSD1306Wire.h"

/**
 *  Testin Requirements: 
 * All testing requires visual inspection of the Serial log during
 * the testing. To see the appropriate debug messages
 * debug level must be set == FREQ_AND_DIO
 * 
 * The easist way to see changes is to look for
 * "TX on freq  .... " within the serial port output data
 *
 * If you uncomment the ADR test code mentioned below you should be able
 * to run this start to finish. This should exit when test is complete
 * 
 *
 *  TESTS
 * 
 *  1. ADR test: 
 *  Enables ADR and with help of debug code within
 *  the runtime, we will fake out the device data rate change
 *  that should occur when no response is recieved from the network
 *  for an extended time period.
 *  In order for the ADR test to run there is commented out 
 *  runtime code that needs to be enabled. See "bool testADR()"
 *  below for details.
 * 
 *  2. Transmit power test:
 *  Sets transmit power from TX_POWER_0 to TX_POWER_10 in increments of +1
 *  max power should be 20 dBm, min 10 dBm
 * 
 *  3. Set Data Rate test.  Changes the data rate, sends one packet 
 *  whose length is less than the DR maximum and one that exceeds
 *  the DR maximum. First should succeed, second should fail without
 *  sending a zero length if no MAC response is pending
 */
// 
//  
//  ADR testing
//  verify Data/rate/ TxPower change if ADR enabled and 
//  count overriden in LoRaMac.cpp

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

uint32_t delayTime = 10000; //60000;
uint32_t lastTime = 0;
bool sendIt = false;

// Robs Keys


/* OTAA para*/
uint8_t devEui[] = { 0x2D, 0x0F, 0x60, 0x6B, 0x10, 0xB9, 0x3F, 0x6F };
uint8_t appEui[] = { 0x67, 0x84, 0x53, 0xFD, 0x56, 0x07, 0x9A, 0x3F };
uint8_t appKey[] = { 0xEE, 0x8C, 0xCF, 0xF0, 0x42, 0x67, 0x34, 0xE1, 0x31, 0x29, 0xD5, 0x01, 0x1D, 0xD9, 0x75, 0x58 };


/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0xFF00,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools or platformio.ini*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 20000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 4;

// access to the display as initialized by the LoRaWan object
extern SSD1306Wire  display;
void displayCustomString( char * custString);
uint16_t sendCount = 0;
int8_t powerLevel = TX_POWER_6;   // MAX for US915
int8_t dataRate = DR_0;
bool getAdr();
void setAdr(bool);
int8_t getDataRate();
void setDataRate(int8_t rate);
int8_t getTxPowerLevel();
void setTxPowerLevel(int8_t powerLevel);
bool chgDataRate();
int8_t numDataRateTests=0;
bool chgTxPower();
int8_t numTxPowerTests=0;
bool testADR();
int8_t numADRTests=0;
int8_t testNumber = 1;
int8_t getJoe();

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
  /*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
  *appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
  *if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
  *if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
  *for example, if use REGION_CN470, 
  *the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
  */
   
    appData[0] = 0x00;
    appData[1] = 0x01;
    appData[2] = 0x02;
    appData[3] = 0x03;
}


void setup() {
  boardInitMcu();
  Serial.begin(115200);
  delay(3000);
  Serial.println("\n\n\tApp Name: Helium Example");

  display.init();
  display.clear();
  displayCustomString("This is my test");
  
#if(AT_SUPPORT)
  enableAt();
#endif
  deviceState = DEVICE_STATE_INIT;
  LoRaWAN.ifskipjoin();
  // initial default packet size
  appDataSize = 1;
}

void loop()
{
  uint32_t currTime = millis();

  if ( currTime > lastTime + delayTime)
  {
    sendIt = true;
    lastTime = currTime; 
  }
  
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(AT_SUPPORT)
      getDevParam();
#endif
  
      printDevParam();
     
      LoRaWAN.init(loraWanClass,loraWanRegion);

      // set default to DR_0
      //LoRaWAN.setDataRateForNoADR(DR_0);

      // If you reset Data rate or transmit power, be sure
      // it is after the LoRaWAN.init().

      Serial.print("Main: Get default starting data rate: ");
      Serial.println(LoRaWAN.getDataRateForNoADR());

      // // default DR
      // LoRaWAN.setDataRateForNoADR(DR_3);

      // Serial.print("Main: Get new data rate after change: ");
      // Serial.println(LoRaWAN.getDataRateForNoADR());

      deviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      
      break;
    }
    case DEVICE_STATE_SEND:
    {
    
      if (sendIt == false)
      {
        break;
      } else {
        sendIt = false;
      }
      
      sendCount++;
      Serial.print("\n\tMain: sending - Count: ");
      Serial.println(sendCount);
      Serial.println("\n");

      bool done = false;
      switch(testNumber)
      {
        case 1:
          Serial.println(">> Testing ADR >>");
          done = testADR();
          break;
        case 2:
          Serial.println(">> Testing Power change >>");
          done = chgTxPower();
          break;
        case 3:
          Serial.println(">> Testing Data Rate Change >>");
          done = chgDataRate();
          
          break;
        case 4:
        Serial.println(">>>>> ALL TESTING IS COMPLETE <<<<<");
          exit(1);
          break;
      }
      
      if(done)
      {
        testNumber++;
        sendCount = 0;
      }
      prepareTxFrame( appPort );
  
      Serial.print("Main:  packet size: ");
      Serial.println(appDataSize);
      Serial.print("\tmain packet data\r\n\t");
      for (uint8_t i = 0; i < appDataSize; i++) {
        Serial.print(appData[i]);
      }
      Serial.println("");


      LoRaMacStatus_t sendStatus = LoRaWAN.send();

			if (sendStatus != LORAMAC_STATUS_OK) {
				Serial.println("Main: ERROR - send failed: ");
        Serial.println(sendStatus);
			} else {
        //printf("Main: - send success: %d\r\n", sendStatus);
        Serial.println("Main - send success: ");
        Serial.println(sendStatus);
			}
			//printf("\n\n");
      Serial.println("");

      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( 0, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.sleep();
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}
bool chgTxPower()
{
  bool done = false;
  int8_t pwr= TX_POWER_0;

  switch (sendCount)
  {
    case 1:
      Serial.println(">>>>> setTxPowerLevel test starting... >>>>>");
      Serial.println("");

      pwr= TX_POWER_0;        
      setTxPowerLevel(pwr);
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 20 dBm\r\n", pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 20 dBm");
    break;
    case 2:
      pwr= TX_POWER_1;        
      setTxPowerLevel(pwr);
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 20 dBm\r\n", pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 20 dBm");
      break;
    case 3:
      pwr= TX_POWER_2;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 20 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 20 dBm\r\n", pwr);
      break;
    case 4:
      pwr= TX_POWER_3;
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 20 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 20 dBm\r\n", pwr);
      break;
    case 5:
      pwr= TX_POWER_4;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 20 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 20 dBm\r\n", pwr);
      break;
    case 6:
      pwr= TX_POWER_5;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 20 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 20 dBm\r\n", pwr);
      break;
    case 7:
      pwr= TX_POWER_6;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 18 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 18 dBm\r\n", pwr);
      break;
    case 8:
      pwr= TX_POWER_7;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 16 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 16 dBm\r\n", pwr);
      break;
    case 9:
      pwr= TX_POWER_8;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 14 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 14 dBm\r\n", pwr);
      break;
     case 10:
      pwr= TX_POWER_9;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 12 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 12 dBm\r\n", pwr);
      break;
     case 11:
      pwr= TX_POWER_10;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(", \"TX on freq\" should be 10 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 10 dBm\r\n", pwr);
      break;
    case 12:
      pwr= TX_POWER_11;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.println(pwr);
      Serial.println(" set should fail, exceeds max power setting");
      Serial.println(" \"TX on freq\" should be 10 dBm");
     // printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 10 dBm\r\n", pwr);
      break;
    case 13:
      pwr= TX_POWER_12;        
      setTxPowerLevel(pwr);
      Serial.print(">>> setTxPowerLevel: ");
      Serial.print(pwr);
      Serial.println(" set should fail, exceeds max power setting");
      Serial.println(" \"TX on freq\" should be 10 dBm");
      //printf(">>>>> setTxPowerLevel: %d, Radio \"TX on freq\" should be 10 dBm\r\n", pwr);    
      break;
    case 14:
      // reset to max power
      pwr= TX_POWER_0;        
      setTxPowerLevel(pwr);
      Serial.println("");
      Serial.println(">>>>> setTxPowerLevel test complete... <<<<<");
      Serial.println("");
      done = true;
      break;
  }
  
  return done;
}

// for ADR test you must enable code marked "Useful for ADR ON"
// at several locations within LoRaMac.c and RegionUS915.c
bool testADR() 
{
  bool done = false;

  switch (sendCount)
  {
    case 1:
      Serial.println("");
      Serial.println(">>>>> ADR test starting... <<<<<");
      Serial.println("");
      Serial.println("Set ADR ON");
      setDataRate(DR_3);    // set high DR to start
      setAdr(true); 
      getAdr();
    break;
    case 10:
      // the complete ADR cycle should have been completed by now.
      // in serial output you should see adrAckCounter == 96 at 
      // send count of 4, from then on data rate should be changed for 
      // each send.
      Serial.println("");
      Serial.println(">>>>> ADR test complete... <<<<<");
      Serial.println("Set ADR OFF");
      setAdr(false);
      done = true;
    // break;
  }

  return done;
}

// Change Data Rate test
// For each DR setting we send a valid length packet followed
// by one that exceeds the DR maximum size. The first should
// succeed. the second should fail to send and "NOT" send a zero
// length packet as there should be no MAC response to send.
bool chgDataRate()
{
  bool done = false;
  switch (sendCount)
  {
    // note: We do not start this test until send number 3 to
    // allow the network a chance to send and the runtime to
    // process SRV_MAC_LINK_ADR_REQ.
    case 3:
        Serial.println("");
        Serial.println(">>>>> Set Data Rate test starting... <<<<<");
        setDataRate(DR_0);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_0, packet size 1");
        Serial.println(">>>>>>>>>>>> Should succeed");
        appDataSize = 1;
    break;
    case 5:
        setDataRate(DR_0);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_0, packet size 12");
        Serial.println(">>>>>>>>>>>> too large should fail");
        appDataSize = 12;
    break;
    case 7:
        setDataRate(DR_1);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_1, packet size 12");
        Serial.println(">>>>>>>>>>>> Should succeed");
        appDataSize = 12;
    break;
    case 9:
        setDataRate(DR_1);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_1, packet size 54");
        Serial.println(">>>>>>>>>>>> too large Should Fail");
        appDataSize = 54;
    break;
    case 11:
        setDataRate(DR_2);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_2, packet size 54");
        Serial.println(">>>>>>>>>>>> Should succeed");
        appDataSize = 54;
    break;
    case 13:
        setDataRate(DR_2);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_2, packet size 126");
        Serial.println(">>>>>>>>>>>> too large Should Fail");
        appDataSize = 126;
    break;
    case 15:
        setDataRate(DR_3);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_3, packet size 126");
        Serial.println(">>>>>>>>>>>> Should succeed");
        appDataSize = 126;
    break;
    case 17:
        setDataRate(DR_3);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_3, packet size 243");
        Serial.println(">>>>>>>>>>>> too large Should Fail");
        appDataSize = 243;
    break;
    case 19: 
        setDataRate(DR_4);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_4, packet size 1");
        Serial.println(">>>>>>>>>>>> Should succeed");
        appDataSize = 1;
    break;
    case 21:
        setDataRate(DR_4);
        Serial.println(">>>>>>>>>>>> Set data rate to DR_4, packet size 243");
        Serial.println(">>>>>>>>>>>> too large Should Fail");
        appDataSize = 243;
    break;

    case 23:
        Serial.println("");
        Serial.println(">>>>> Set Data Rate test complete... <<<<<");
        Serial.println("");
        done = true;
    break;

  }
  return done;

}
// Display a custom string, callable from the app level
void displayCustomString( char * custString)
{
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.clear();
        display.drawStringMaxWidth(0, 1, 128, custString);
        display.display();
}



bool getAdr()
{
  // check state of the Adaptive Data Rate flag
  MibRequestConfirm_t mibReq;
  mibReq.Type = MIB_ADR;

  if( LoRaMacMibGetRequestConfirm( &mibReq ) == LORAMAC_STATUS_OK )
  {
    Serial.print("ADR Enabled Results from MibGet: ");
    Serial.println(mibReq.Param.AdrEnable);
    return mibReq.Param.AdrEnable;
  }
}

void setAdr(bool value)
{
  // check state of the Adaptive Data Rate flag
  MibRequestConfirm_t mibReq;
  mibReq.Type = MIB_ADR;
  mibReq.Param.AdrEnable = value;

  eLoRaMacStatus status =  LoRaMacMibSetRequestConfirm( &mibReq );
  if(status == LORAMAC_STATUS_OK )
  {
    Serial.println("set ADR Successful");
  } else {
    Serial.println("Error: set ADR failed: error code: %d");
    Serial.print(status);
  }
}

int8_t getDataRate()
{
  int8_t rate = LoRaWAN.getDataRateForNoADR();
  Serial.print("Get Current Data Rate: ");
  Serial.println(rate);
  return rate;
}

void setDataRate(int8_t rate)
{
  Serial.print("Set Data Rate to: ");
  Serial.println(rate);
  LoRaWAN.setDataRateForNoADR(rate);
}

int8_t getTxPowerLevel()
{
  // // get the current power level
  MibRequestConfirm_t mibReq;
  eLoRaMacStatus status;

  mibReq.Type = MIB_CHANNELS_TX_POWER;

  status = LoRaMacMibGetRequestConfirm( &mibReq ); 
  
  if( status == LORAMAC_STATUS_OK )
  {
      Serial.print("Get Current Transmit Power Level: ");
      Serial.println(mibReq.Param.ChannelsTxPower);
  } else {
      Serial.print("Fetch of current power level failed: ");
      Serial.println( status);
  }
  
  return mibReq.Param.ChannelsTxPower;
}

 // if you want to reset the transmit power level on
      // the third send
      // Be carefull, the network sends a LinkADRReq MAC command
      // sometime after the first data uplink. The processing of 
      // MAC command will set the date rate and transmit power. 
      // the data rate used on the uplink is reflected back, thus
      // if you change it prior to the first uplink it is essentially
      // preserved.
      // As the network has no knowlege of the transmit power used
      // it defaults to suggesting max power
void setTxPowerLevel(int8_t powerLevel)
{
  MibRequestConfirm_t mibReq;
  eLoRaMacStatus status;
  // reset the transmit power
  mibReq.Type = MIB_CHANNELS_TX_POWER;
  mibReq.Param.ChannelsTxPower = powerLevel;

      status = LoRaMacMibSetRequestConfirm( &mibReq ); 
  if( status == LORAMAC_STATUS_OK )
  {
      Serial.print("Tx power updated to: ");
      Serial.println(powerLevel);
  } else {
      Serial.print("Tx power update failed: ");
      Serial.println(status);
  }
}     
