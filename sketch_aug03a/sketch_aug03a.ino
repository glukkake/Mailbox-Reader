int sensorValue;
 
void setup()
{
  Serial.begin(115200); // starts the serial port at 9600
}
 
void loop()
{
  sensorValue = analogRead(A1); // read analog input pin 0
  Serial.print(sensorValue, DEC); // prints the value read
  Serial.print(" \n"); // prints a space between the numbers
  delay(1000); // wait 100ms for next reading
}