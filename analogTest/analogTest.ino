/******************************************************************************
SoftPot_Example.ino
Example sketch for SparkFun's soft membrane potentiometer
  (https://www.sparkfun.com/products/8680)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

- Connect the softpot's outside pins to 5V and GND (the outer pin with an arrow
indicator should be connected to GND). 
- Connect the middle pin to A0.

As the voltage output of the softpot changes, a line graph printed to the
serial monitor should match the wiper's position.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int p = A0; // Pin connected to softpot wiper
int state = 0;
int lastVal = 0;
int num = 2;
int baseVal = 0;
int curVal = 0;
float aveVal = 0;

void setup() 
{
  Serial.begin(115200);
  pinMode(p, INPUT);
  pinMode(0, OUTPUT);
  baseVal = analogRead(p);
}

void loop() 
{  
  /*Serial.println(String(curVal));
  if (curVal - lastVal > 30){
    digitalWrite(0, (state) ? HIGH : LOW);
    state = !state;
  };*/
  curVal = 0;
  aveVal = 0;
  for(int i = 1; i < num; ++i)
  {
    curVal += analogRead(p);
    delay(2);
  }
  aveVal = curVal/(num);
  Serial.println(String(aveVal));
  /*curVal /= num;
  if (curVal - baseVal > 25){
    digitalWrite(0, (state) ? HIGH : LOW);
    state = !state;
  };*/
  //Serial.println(String(curVal));
  delay(50);
}
