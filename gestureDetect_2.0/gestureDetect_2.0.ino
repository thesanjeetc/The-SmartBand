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
const int p = A0;
int state = 0;
int lastVal = 0;
int num = 3;
int baseVal = 0;
int diffVal = 0;
int curVal = 0;
int startVal = 0;
int endVal = 0;
int maxDiff = 0;
int lastDiff = 0;
unsigned long startMillis;
unsigned long currentMillis;
unsigned long diff;
unsigned long dm;
unsigned long cm;
unsigned long sm;

void setup()
{
  Serial.begin(115200);
  pinMode(p, INPUT);
  pinMode(0, OUTPUT);
  baseVal = analogRead(p);
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  diff = currentMillis - startMillis;
  if (diff >= 10) {
    curVal = analogRead(p);
    if (curVal > 30) {
      curVal = analogRead(p);
      Serial.println(String(curVal));
      /*startVal = curVal;
      lastVal = curVal;
      lastDiff = 0;
      maxDiff = 0;
      while (curVal > 30) { // Whilst touching
        curVal = analogRead(p);
        diffVal = curVal - lastVal;
        if (abs(diffVal) > 100) {
          if (abs(maxDiff) < 20) {
            Serial.println(String("Tap"));
          } else if (maxDiff > 0) {
            Serial.println(String("Swipe up"));
          } else {
            Serial.println(String("Swipe down"));
          };
        } else if (abs(diffVal) > abs(lastDiff)) {
          maxDiff = diffVal;
        };*/
        /*Serial.println(String(diffVal));
          Serial.println(String(startVal));
          Serial.println(String(lastVal));
          Serial.println(String(curVal));*/
          lastVal = curVal;
      lastDiff = diffVal;
          /*if (lastDiff > 40){ // If let go
          if(abs(curVal-lastVal) < 15){
            Serial.println(String("Tap"));
          }else{
            if(curVal-lastVal > 0){
              Serial.println(String("Swipe up"));
            }else{
              Serial.println(String("Swipe down"));
            }
          }*/
        //Serial.println(String(curVal));
        //Serial.println(String(startVal));
        //Serial.println(String(lastVal));
      //}
      delay(25);
    }
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}
/*if (startVal - endVal > 0){
  digitalWrite(0, HIGH);
  }else{
  digitalWrite(0, LOW);
  }*/

// Points - Lowered delay to ensure quick recognition
// Tried avaeraging but slowed down - not the problem
// Flow chart diagram
// 2-3 dyas
// changed to use millis() instead of delay - https://learn.adafruit.com/multi-tasking-the-arduino-part-1/using-millis-for-timing
// reduce accidental changes
// state machine !!!!!!!!!!
// testing using different length of pressures
// changed thinking from tap being small time delay to low variation in voltage
// soft wd reset - low delay - wifi stack - timing constraints - delay in wrong place
