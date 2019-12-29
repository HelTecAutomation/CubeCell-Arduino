/****************************************************************
 * ReadSHT2x
 *  An example sketch that reads the sensor and prints the
 *  relative humidity to the PC's serial port
 ***************************************************************/

#include <Wire.h>
#include <SHT2x.h>


void setup()
{
    pinMode(Vext, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    digitalWrite(Vext, LOW);
    delay(50);

    Wire.begin();
    Serial.print("Humidity(%RH): ");
    Serial.print(SHT2x.GetHumidity());
    Serial.print("     Temperature(C): ");
    Serial.println(SHT2x.GetTemperature());
    Wire.end();

    digitalWrite(Vext, HIGH);

    delay(3000);
}
