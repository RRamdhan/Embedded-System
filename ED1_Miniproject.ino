#include <Stepper.h>
#include <Servo.h>
#include <EEPROM.h>

Servo myservo;
int pos = 0;                  //Servo position
int randnumber;
int stepposition;
int servoposition;
const int stepsPerRevolution = 40; 
const int inputPin = 2;           // choose the input pin (for the PIR sensor)


Stepper myStepper(stepsPerRevolution, 12, 13);   


void setup() 
{
  Serial.begin(9600);
  // set the speed of stepper at 10 rpm:
  myStepper.setSpeed(10);
  pinMode(3, OUTPUT);
  digitalWrite (3, HIGH);
  pinMode(11, OUTPUT);
  digitalWrite (11, HIGH);
  myservo.attach(6);              //attaches servo to pin 6 on motorshield
  // initialize the serial port:
  // pinMode(ledPin, OUTPUT);         // declare LED as output
  pinMode(inputPin, INPUT);        // declare pushbutton as input
  // digitalWrite(ledPin, LOW); 
  randomSeed(analogRead(0));
  //Initial servo position
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  myservo.write(30);
  servoposition = 30;
  EEPROM.write(4, servoposition);
}


void static motors() 
{
  ///////////////////////////////////////////////////////////////////////////
  // step one revolution  in one direction:
  randnumber = random(10000);
  
  if (stepposition > 20) 
  {
    Serial.println("Step clockwise");
    myStepper.step(-randnumber%20);
    delay(500);
    stepposition = (stepposition - randnumber%20);
  }
  
    else if (stepposition <= 20) 
  {
    Serial.println("Step counterclockwise");
    myStepper.step(randnumber%20);
    delay(500);
    stepposition = (stepposition + randnumber%20);
  }
  ///////////////////////////////////////////////////////////////////////////
  if (servoposition > 45){
    Serial.println("Servo counterclockwise");
    /* for(pos = servoposition; pos <= servoposition - (randnumber%15); pos -= 1) {
      myservo.write(pos);
      delay(15); */
    servoposition = servoposition - random(10000)%25;
    myservo.write(servoposition);
    //}
  }
  
  else if (servoposition <= 45){
    Serial.println("Servo clockwise");
    /* for(pos = servoposition; pos <= servoposition + (randnumber%15); pos += 1) {
      myservo.write(pos);
      delay(15);
    } */
    servoposition = servoposition + random(10000)%25;
    myservo.write(servoposition);
  }
  EEPROM.write(0, stepposition);
  EEPROM.write(4, servoposition);
}


void loop()
{
  stepposition = EEPROM.read(0);
  servoposition = EEPROM.read(4);
  int val = digitalRead(inputPin);  // read input value
  
 
  
  if (val == HIGH)                  // check if the input is HIGH
  {
    Serial.println("MOTION");
    motors();
    //digitalWrite(ledPin, HIGH);     // turn LED on if motion detected
    delay(5);
    //digitalWrite(ledPin, LOW);      // turn LED off
  }
}

