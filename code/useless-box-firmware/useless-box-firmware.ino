#include <A4988.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <DRV8834.h>
#include <DRV8880.h>
#include <MultiDriver.h>
#include <SyncDriver.h>

#include <Servo.h>

#define NUM_SWITCHES 5
#define TOP_POS 30
#define STANDBY_POS 90
#define OFF_POS 140
#define RPM 200
#define MICRO_STEPPING 16

#define STEPS_PER_REV 200

#define HOMEING_MOVEMENTS 1

A4988 stepper(STEPS_PER_REV, 0,1, 4, 3, 2);

Servo* servos[2];
short servoPins[] = {9,10};
short servoPinsc = 2;
short switchPins[] = {2,4,7,8,12};
short switchPinsc = 5;
short endStopPin = 12;

short switchesState[NUM_SWITCHES]={0,0,0,0,0};
unsigned int switchesPos[NUM_SWITCHES] = {10,20,30,40,50};

short isHomed = 0;

void setup() {
  // put your setup code here, to run once:
  // setup switches as digital inputs
  pinMode(endStopPin, INPUT_PULLUP);
  //for(int i=0;i<switchPinsc;++i)
    //pinMode(switchPins[i],INPUT);

  //create servo objects and attach pwm pins
  for(int i=0;i<servoPinsc;++i){
    servos[i] = new Servo();
    servos[i]->attach(servoPins[i]);
  }
  
  // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
  stepper.begin(RPM/4, MICRO_STEPPING);

}


int goToHome(){
  isHomed = 0;
  while(!isHomed){
    if(!digitalRead(endStopPin)){
      isHomed = 1;
      return 1;
    }
    stepper.rotate(0.9);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
    //goToHome();
    //delay(1000); 
    //stepper.rotate(-1070);
    delay(2000);   
    servos[0]->write(STANDBY_POS);              // tell servo to go to position in variable 'pos'
    stepper.rotate(1070/2);
    delay(2000);  
    servos[0]->write(OFF_POS);              // tell servo to go to position in variable 'pos'
    delay(2000);   
    servos[0]->write(STANDBY_POS);              // tell servo to go to position in variable 'pos'
    delay(2000);  
    servos[0]->write(TOP_POS);
}
