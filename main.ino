#include <Servo.h>

//sets up servos
Servo servoLeft;
Servo servoRight;


//set up global speed variables

int fast = 90;
int slow = 115;
int stp = 106;

//assigns pingSensorPins
const int pingPinF = 7;
const int pingPinL = 8;
const int pingPinR = 6;

//constants for IR detection
const int rightirLed = 4;
const int rightirReceiver = 5;
const int leftirLed = 12;
const int leftirReceiver = 13;
const int freq = 38000;

//photoresistor constants
int sensorPin = A4;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

//determines what mode
bool obstacleMode = true;
bool isFirstTime = true;

void setup() { 
  //setup serial monitor
  Serial.begin(9600);
  
  //setup movement servos
  servoLeft.attach(11);
  servoRight.attach(10);
  //servoLeft.write(55);
  //servoRight.write(90);
  servoLeft.write(stp);
  servoRight.write(stp);

  //setup IRsensors
  pinMode(5, INPUT);
  pinMode(4, OUTPUT);
  pinMode(13, INPUT);
  pinMode(12, OUTPUT);
  
}

void loop() {
  //if still in obstacle avoidance mode continue
  //if not skip to light seeking mode
  if(obstacleMode){
    //determines if it is dark enough to switch
    //to light seek mode
    obstacleMode = photoDetector();
  }
  //obstacle mode
  if(obstacleMode){
    //is anything in front?
    //no
    if (pingSensor(pingPinF) > 12){
      //anything to the right?
      if (pingSensor(pingPinR) < 6){
        left45();
      }
      //anything to the left?
      else if (pingSensor(pingPinL) <8){
        right45();
      }
      //black line to the right?
      else if (irDetect(rightirLed, rightirReceiver, freq) == 1){
          if (irDetect(rightirLed, rightirReceiver, freq) == 1){
            left45();
          }
      }
      //black line to the left?
      else if (irDetect(leftirLed, leftirReceiver, freq) == 1){
          if (irDetect(leftirLed, leftirReceiver, freq) == 1){
            right45();
          }
        
      }
      //if no obstacles continue
      moveForward();
    }
    //yes 
    else {
      //is anything to the right?
      //no
      if (pingSensor(pingPinR) > 12){
        //avoidance path
        right90();
        move1FootForward();
        move1FootForward();
        left90();
        move1FootForward();
        move1FootForward();
        move1FootForward();
        left90();
        move1FootForward();
        move1FootForward();
        right90();
      } 
      //yes 
      else {
        //is anything to the left?
        //no
        if (pingSensor(pingPinL) > 12){
        left90();
        move1FootForward();
        move1FootForward();
        right90();
        move1FootForward();
        move1FootForward();
        move1FootForward();    
        right90();    
        move1FootForward();
        move1FootForward();
        left90();
        }
        //yes
        else {
          //shift slightly left
          left45();
          if (pingSensor(pingPinF) > 12){
            move1FootForward();
            right45();
          } else {
            right90();
            if (pingSensor(pingPinF) > 12){
              move1FootForward();
              left45();
            } else {
              left45();
              move4FeetForward();
            }
          }
        }
      }
    }
   }
   //light seeking mode
   else{
    //beep if first light seeking iteration
    if (isFirstTime){
      tone(3,2000,300);
      delay(1000);
      tone(3,2000,300);
      delay(1000);
      move4FeetForward();
      isFirstTime = false;
    }
    //is something in front of it?
    //no - start search
    else {
      //rotates right, scans left
      //when reaches an obstacle
      //stops and beeps in happiness
      didIWin();
      right90();
      didIWin();
      float lastVal = photoValueDetector();
      left45();
      float curVal = photoValueDetector();
      while (curVal > lastVal){
        lastVal = curVal;
        left45();
        curVal = photoValueDetector();
        didIWin();
      }
      right45();
      didIWin();
      move4FeetForward();
    }
   }
} 

//--did I win?--//

void didIWin(){
   if (pingSensor(pingPinF) < 12){
    //yes - beep happily forever
    while (true){
      tone(3,5000,300);
      delay(1000);
      tone(3,3000,300);
      delay(1000);
      tone(3,1000,300);
      delay(1000);
    }
   }
   
}

//-----Movement Controls-----//
void moveForward() {
  // move approx one foot forward
  //servoLeft.write(slow);
  //servoRight.write(slow);
  servoLeft.write(100);
  servoRight.write(100);
  delay(200);
  servoLeft.write(stp);
  servoRight.write(stp);
  delay(40);
}

void move1FootForward() {
  // move approx one foot forward
  //servoLeft.write(slow);
  //servoRight.write(slow);
  servoLeft.write(100);
  servoRight.write(100);
  delay(1000);
  servoLeft.write(stp);
  servoRight.write(stp);
  delay(20);
  }

void move4FeetForward() {
  // move 4 feet forward
  //servoLeft.write(70);
  //servoRight.write(70);
  servoLeft.write(100);
  servoRight.write(100);
  delay(2800);
  servoLeft.write(stp);
  servoRight.write(stp);
  delay(20);
  }

void left90(){
  //pivot 90 degrees left
  servoLeft.write(115);
  //servoRight.write(slow);
  servoRight.write(95);
  delay(900);
  servoLeft.write(stp);
  servoRight.write(stp);
  delay(20);
  }

void right90(){
  //pivot 90 degrees right
  //servoLeft.write(slow);
  servoLeft.write(95);
  servoRight.write(115);
  delay(900);
  servoLeft.write(stp);
  servoRight.write(stp);
  delay(20);
  }

void left45(){
  //pivot 45 degrees left
  servoLeft.write(115);
  //servoRight.write(slow);
  servoRight.write(95);
  delay(450);
  servoLeft.write(stp);
  servoRight.write(stp);
  delay(20);
}

void right45(){
  //pivot 45 degrees right
  //servoLeft.write(slow);
  servoLeft.write(95);
  servoRight.write(115);
  delay(450);
  servoLeft.write(stp);
  servoRight.write(stp);
  delay(20);
}

//-------Ping Sensor------//
long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

long pingSensor (int pingPin){
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in");
  //Serial.print(cm);
  //Serial.print("cm");
  Serial.println();
  return inches;
}

//------IR Sensor-----//

//int irDetectCaller() {
  //irDetect(irLed, irReceiver, freq);
//}

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);//RLED 38 kHz for at least 1 ms
  delay(1);// Wait 1 ms
  int ir = digitalRead(irReceiverPin);// IR receiver -> ir variable
  delay(1);// Down time before recheck
  return ir;// Return 1 no detect, 0 detect
}

void irTest(){
    //tests ir sensors
    int count = 0;
    while (true){
    int irRight = irDetect(rightirLed, rightirReceiver, freq);
    int irLeft = irDetect(leftirLed, leftirReceiver, freq);
    Serial.println(derp);
    Serial.print("right =");
    Serial.println(irRight);
    Serial.print("left =");
    Serial.println(irLeft);
    delay(250);
    count++;
    }
  }


//-------Photoresistor-------//
bool photoDetector() {
  //returns true if it is dark enough
  //to switch into light seeking mode
  sensorValue = analogRead(sensorPin);
  float voltage = sensorValue * (5.0 / 1023.0); 
  if (voltage < 1) {
    return false;
  }
  Serial.println(voltage);
  return true;
}

float photoValueDetector() {
  //returns the value of the photoresistor
  sensorValue = analogRead(sensorPin);
  float voltage = sensorValue * (5.0 / 1023.0); 
  Serial.println(voltage);
  return voltage;
}

