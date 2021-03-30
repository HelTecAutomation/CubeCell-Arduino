/*this code is the second UART for cubecell AB02, AB02S, AB02A*/

void setup() {
  // put your setup code here, to run once:
Serial1.begin(115200);//open the second UART
}

void loop() {
  // put your main code here, to run repeatedly:
Serial1.println("hello");
delay(1000);
}
