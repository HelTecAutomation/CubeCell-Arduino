void DownLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    if (mcpsIndication->Buffer[i] == 220) { // DC for APP_TX_DUTYCYCLE; 0D BB A0  for 900000 (15min); 04 93 E0 for 300000 (5min)
      appTxDutyCycle = mcpsIndication->Buffer[i++]<<32|mcpsIndication->Buffer[i++]<<16|mcpsIndication->Buffer[i++]<<8|mcpsIndication->Buffer[i++];
      Serial.print("  new DutyCycle received: ");
      Serial.print(appTxDutyCycle);
      Serial.println("ms");
      saveDr();
    }
  }
}