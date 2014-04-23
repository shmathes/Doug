//This is the library for the IR sensor, it can be found at https://code.google.com/p/gp2y0a21yk-library/
#include <DistanceGP2Y0A21YK.h>
//This is the library for the ping sensor, it can be found at https://code.google.com/p/arduino-new-ping/
#include <NewPing.h>


//Setting up global variable for IR sensors
DistanceGP2Y0A21YK Dist;
/*
This is setting up the global variables for the right and left ping sensors.
The first two numbers are the pins the sensor is connected to on the Arduino Mega ADK board.

Note: I am unsure what the 300 means.
*/
NewPing leftSonar(24,22,300);
NewPing rightSonar(48,46,300);

//These are just setting the global variables for the left and right ping sensors input, and the IR sensors input.
int leftDistance, rightDistance;
int irDistance;

void setup() 
{
  /*
  Where I am calling the Serial.begin(9600) will allow us to view outputs from the variables Doug is creating
  You can view the serial objects by clicking the top drop box 'Tools' and then clicking 'Serial Monitor' or by pressing 'ctrl + shift + m'
  In void loop() I could place the line 'Serial.println(leftDistance)' and it will print out the variable leftDistance to the serial monitor each time that the method runs through
  
  Note: I am unsure why we call it on 9600
  */
  Serial.begin(9600);
  /*
  All of the pinMode methods setup where the code will be pipping output to on the Arduino Mega board.
  */
  //Motor A --- Left motor
  pinMode(12 , OUTPUT);
  pinMode( 9, OUTPUT);
  pinMode(3, OUTPUT);
  
  //Motor B --- Right motor
  pinMode(13 , OUTPUT);
  pinMode( 8, OUTPUT);
  pinMode(11 , OUTPUT);
  
  //green lights
  pinMode(26, OUTPUT);
  pinMode(34, OUTPUT);
  //red lights
  pinMode(28, OUTPUT);
  pinMode(36, OUTPUT);
  
  //this is just seting our global variables to 0 before the main loop is called.
  leftDistance = 0;
  rightDistance = 0;
  irDistance = 0;
}
void loop() 
{
  getDistance();
  //as stated above when we setup the Serial.begin(9600) these will dispaly outputs of these two variables to the serial monitor
  Serial.println(rightDistance);
  Serial.println(leftDistance);
  
  forward();
  path_clear();
  if(rightDistance < 40 || leftDistance < 40)
  {
      if(leftDistance > rightDistance)
      {
         right_blocked();
         turnLeft();
      }
      else if(leftDistance < rightDistance)
      {
         left_blocked();
         turnRight();
      }
      else
      {
         path_blocked();
         reverse();
      }
  }
}//end loop()
/*
These are the movement methods, and I think their names make them very easy to work with. If the analogWrite for pin 3 or 11 is at 255, it will move at its top speed, if it is at 0 it will stop moving.
The digitalWrites correspond to the pins we declared as motors in the setup() method in the beginning of the code.
*/
void reverse()
{
  digitalWrite(12, LOW);
  digitalWrite(9, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(8, LOW);
  analogWrite(11, 255);
  analogWrite(3, 240); 
}//end backward()
void brake()
{
    analogWrite(3, 0);
    analogWrite(11, 0);
    digitalWrite(8, HIGH);
    digitalWrite(9, HIGH);
}//end brake()
void forward()
{
    digitalWrite(12, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(8, LOW);
    analogWrite(11, 255);
    analogWrite(3, 255); 
}//end forward()
void turnRight()
{
    digitalWrite(12, LOW);
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(8, LOW);
    analogWrite(11, 255);
    analogWrite(3, 255); 
}//end turn_right()
void turnLeft()
{
    digitalWrite(12, HIGH);
    digitalWrite(9, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(8, LOW);
    analogWrite(11, 255);
    analogWrite(3, 255); 
}//end turn_left()
/*
The methods below are just the displaying the lights, the names are self explanitory.
if path_clear() is called, both lights will be green
if path_blocked() is called, both lights will be red
if right_blocked() is called, the right sides red light will be on, and the left sides green light will be on
if left_blocked() is called, the right sides green light will be on, and the left sides red light will be on
Since the pins being written to are on the digital output pins on the board, they will only have values of 'LOW' or 'HIGH' which just means off and on. 

Note: You can refer back to the setup method where we declared where these pins were on our board.
*/
void path_clear()
{
    digitalWrite(28, LOW);
    digitalWrite(36, LOW);
    digitalWrite(26, HIGH);
    digitalWrite(34, HIGH);
}
void path_blocked()
{
    digitalWrite(36, HIGH);
    digitalWrite(28, HIGH);
    digitalWrite(26, LOW);
    digitalWrite(34, LOW);
}
void right_blocked()
{
    digitalWrite(36, LOW);
    digitalWrite(28, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(34, LOW);
}
void left_blocked()
{
    digitalWrite(36, HIGH);
    digitalWrite(28, LOW);
    digitalWrite(26, LOW);
    digitalWrite(34, HIGH);
}
/*
With the metod getDistance() you must call a delay of atleast 50 between getting readings from the ping sensors. If you call right sensor and left senors in rapid
succession, the second sensor called will recieve interference from the first.
This method first gets leftDistance by calling the left ping sensor named 'leftSonar' and using the ping_cm() method in the sonar library. The same is done for the right method.

Note: There was a problem getting the inital values from the ping sensors, that is why we have the if statements of 'if(someDistance == 0) someDistance = 300;' There maybe a work around to this, but I have not tried anything with it yet.
*/
void getDistance()
{
   delay(50);
   leftDistance = leftSonar.ping_cm();
   if(leftDistance == 0) leftDistance = 300;
   delay(50);
   rightDistance = rightSonar.ping_cm();
   if(rightDistance == 0) rightDistance = 300;
}

