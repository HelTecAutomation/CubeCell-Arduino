/* Heltec Automation HC-SR04 ultrasonic deistance sensors test example
 *
 * Function:
 * Read distance data from HC-SR04
 *
 * HelTec AutoMation, Chengdu, China
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/ASR650x-Arduino
 
 * Use interrupt function to achieve
 */

#define trigPin GPIO1
#define echoPin GPIO2

float distance;
float pulse_width;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200); // Starts the serial communication
}

uint32_t out_flag_1 = 0;
uint32_t out_flag_2 = 0;
uint32_t t1;
uint32_t t2;

void time1()
{
    out_flag_1 = 1;
    t1 = micros();
}

void time2()
{
    out_flag_2 = 1;
    t2 = micros();
}

void loop() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    attachInterrupt(echoPin,time1,RISING);

     while(1)
     {
        delayMicroseconds(1);
        if(out_flag_1)
        {
          out_flag_1 = 0;
          detachInterrupt(echoPin);
          delayMicroseconds(1);
          attachInterrupt(echoPin,time2,FALLING);
        }
        if(out_flag_2)
        {
          out_flag_2 = 0;
          detachInterrupt(echoPin);
          break;
        }
      }

    pulse_width = t2 - t1;
    distance=pulse_width*0.034/2;
    // Prints the distance(The unit is centimeter) on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    delay(1000);
}
