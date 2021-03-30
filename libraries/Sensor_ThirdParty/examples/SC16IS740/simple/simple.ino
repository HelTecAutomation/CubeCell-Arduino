#include "SC16IS740RK.h"

SC16IS740 extSerial(Wire, 0);

char out = ' ';

void setup() {
	Serial.begin(115200);

	pinMode(Vext, OUTPUT);
  	digitalWrite(Vext, LOW);

	delay(5000);

	extSerial.begin(9600);
}

void loop() {
	while(extSerial.available()) {
		int c = extSerial.read();
      	Serial.print("received ");
		Serial.println(c);
	}

	extSerial.print(out);
	if (++out >= 127) {
		out = ' ';
	}
	delay(100);
}
