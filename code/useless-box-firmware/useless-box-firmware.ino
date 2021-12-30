#include <A4988.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <DRV8834.h>
#include <DRV8880.h>
#include <MultiDriver.h>
#include <SyncDriver.h>

#include <Servo.h>

#define TOP_POS 30
#define STANDBY_POS 90
#define OFF_POS 140


//stepper settings
#define RPM 200
#define STEPS_PER_REV 200
#define MICRO_STEPPING 16
#define HOMEING_MOVEMENTS 1
#define DIR_PIN 11
#define STEP_PIN 10
#define MS1_PIN 9
#define MS2_PIN 8
#define MS3_PIN 7
#define ENABLE_PIN 6
#define MIN_POS -1070
#define MAX_POS 0

//switches
#define SW1_PIN A1
#define SW2_PIN A2
#define SW3_PIN 2
#define SW4_PIN 4
#define SW5_PIN 12
#define SW6_PIN 13
#define NUM_SWITCHES 6
//#define QUEUE_SIZE 7  //NUM_SWITCHES+1

//servos
//PWM pins
#define SERVO1_PIN 3
#define SERVO2_PIN 5
#define NUM_SERVOS 2
#define SERVO_DELAY 250

//endstop
#define ENDSTOP_PIN A0


A4988 stepper(STEPS_PER_REV, DIR_PIN, STEP_PIN, MS1_PIN, MS2_PIN, MS3_PIN);

Servo* servos[NUM_SERVOS];
short servoPins[] = {SERVO1_PIN,SERVO2_PIN};

short switchPins[] = {SW1_PIN,SW2_PIN,SW3_PIN,SW4_PIN,SW5_PIN,SW6_PIN};

short switchesState[]={0,0,0,0,0,0};
unsigned int switchesPos[] = {-1070,-856,-642,-428,-214,0};

short isHomed = 0;
int currentPos = 0;
short isClosed = 1;
short isStandby = 0;
short isExtended = 0;


short queue[NUM_SWITCHES] = {-1,-1,-1,-1,-1,-1};
short queuec = 0;



void setup() {
  // put your setup code here, to run once:
  // setup switches as digital inputs
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);
  for(int i=0;i<NUM_SWITCHES;++i)
    pinMode(switchPins[i],INPUT_PULLUP);

  //create servo objects and attach pwm pins
  for(int i=0;i<NUM_SERVOS;++i){
    servos[i] = new Servo();
    servos[i]->attach(servoPins[i]);
  }
  
  // Set target motor RPM to 1RPM and microstepping to 1 (full step mode)
  stepper.begin(RPM, MICRO_STEPPING);

  //Serial.begin(9600);

  servos[0]->write(OFF_POS);
}


int goToHome(){
  isHomed = 0;
  while(!isHomed){
    if(!digitalRead(ENDSTOP_PIN)){
      isHomed = 1;
      currentPos=0;
      return 1;
    }
    stepper.rotate(0.9);
  }
}

//goto position (pos) given in absolute coordinates
int goTo(int pos){
  if(!isHomed || pos< MIN_POS || pos > MAX_POS)
    return 0;

  int dir = (pos-currentPos)>0?1:-1;
  //check if switch is flipped while moving...
  while(currentPos != pos){
    addSwitchesToQueue();
    stepper.rotate(dir);
    currentPos+=dir;
  }
  return 1;
}
//check if item is in queue
int isInQueue(short item){
  for(int i=0; i< queuec;++i){
    if(queue[i] == item)
      return 1;
  }
  return 0;
}
//add item to end of queue
int qpush(short item){
  queue[queuec++]=item;
}
//remove item from front of queue
int qpop(){
  queuec-=1;
  for(int i=0;i<queuec;++i){
    queue[i] = queue[i+1];
  }
}

int addSwitchesToQueue(){
  for(int i=0;i<NUM_SWITCHES;++i){
    if(!digitalRead(switchPins[i])){
      //Serial.println("Pin" + String(i) + " pressed!");
      if(!isInQueue(i)){
        //Serial.println("push");
        qpush(i);
      }
    }
  }
  //Serial.println("queuec: " + String(queuec));
  //if(queuec>0){
  //  for(int i=0;i<queuec-1;++i)
  //    Serial.print(String(queue[i]) + ", ");
  //  Serial.println(queue[queuec-1]);
  //}
}

int openDoor(){
  //servo 1 open door
  delay(SERVO_DELAY);
  //servo0 to standby position
  servos[0]->write(STANDBY_POS);
  delay(SERVO_DELAY);
  isClosed=0;
  return 1;
}

int closeDoor(){
  //servo0 to off position
  servos[0]->write(OFF_POS);

  delay(SERVO_DELAY);
  //servo 1 close door

  
  delay(SERVO_DELAY);
  isClosed=1;
  return 1;
}

int extend(){
  servos[0]->write(TOP_POS);
  delay(SERVO_DELAY);
}

int retract(){
  servos[0]->write(STANDBY_POS);
  delay(SERVO_DELAY);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
    if(!isHomed)
      goToHome();
  
  addSwitchesToQueue();
  if(queuec){
    if(isClosed)
      openDoor();
    goTo(switchesPos[queue[0]]);
    extend();
    retract();
    qpop();
  }
  addSwitchesToQueue();

  if(!queuec && !isClosed){
    closeDoor();
  }




    //delay(3000); 
    //goTo(-500);
    //delay(3000); 
    //goTo(-1070);
    //delay(2000);   
    //servos[0]->write(STANDBY_POS);              // tell servo to go to position in variable 'pos'
    //stepper.rotate(1070/2);
    //delay(2000);  
    //servos[0]->write(OFF_POS);              // tell servo to go to position in variable 'pos'
    //delay(2000);   
    //servos[0]->write(STANDBY_POS);              // tell servo to go to position in variable 'pos'
    //delay(2000);  
    //servos[0]->write(TOP_POS);
}
