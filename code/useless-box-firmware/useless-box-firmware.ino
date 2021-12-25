#include <Servo.h>

#define NUM_SWITCHEs 5

Servo* servos[2];
short servoPins[] = {9,10};
short servoPinsc = 2;
short switchPins[] = {2,4,7,8,12};
short switchPinsc = 5;
short endStopPin = 13;

short switchesState[NUM_SWITCHES]={0,0,0,0,0};
unsigned int switchesPos = {10,20,30,40,50};

short homed = 0;

void setup() {
  // put your setup code here, to run once:
  // setup switches as digital inputs
  pinMode(endStopPin, INPUT);
  for(int i=0;i<switchPinsc;++i)
    pinMode(switchPins[i],INPUT);

  //create servo objects and attach pwm pins
  for(int i=0;i<servoPinsc;++i){
    servos[i] = new Servo();
    servos[i]->attach(servoPins[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    servos[0]->write(0);              // tell servo to go to position in variable 'pos'
    delay(2000);   
    servos[0]->write(180);              // tell servo to go to position in variable 'pos'
    delay(2000);  
    servos[1]->write(0);              // tell servo to go to position in variable 'pos'
    delay(2000);   
    servos[1]->write(180);              // tell servo to go to position in variable 'pos'
    delay(2000);  
}
