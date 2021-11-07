#include <MIDI.h>
#include "pitches.h"
#include <MD_MAX72xx.h>
#include <SPI.h>
//#include <MD_Parola.h> // uncomment for text effects

/*
 * MIDI STEPPER V1
 * 
 * By Jonathan Kayne / jzkmath
 * April 2018
 * https://github.com/jzkmath/Arduino-MIDI-Stepper-Motor-Instrument
 * 
 * Takes MIDI data and converts it to stepper music!
 * Since the steppers only need to spin in one direction,
 * you only need to control the STEP pin on the A4988 driver
 * and pull DIR to either 5V or GND.
 * You can add more stepper motors to the sketch by specifying more pins
 * and expanding the array.
 * I also tried to put plenty of comments so that you can understand the code
 * and possibly be able to use it or part of it in other projects!
 */
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

//ARDUINO PINS
//configured for CNC Shield V3
#define stepPin_M1 2
#define stepPin_M2 3
#define stepPin_M3 4
#define stepPin_M4 12

#define MAX_DEVICES 4
#define CS_PIN 17
#define DATA_PIN 51
#define CLK_PIN 52

#define enPin 8 //Steppers are enabled when EN pin is pulled LOW

#define TIMEOUT 10000 //Number of milliseconds for watchdog timer

//The index corresponds to the MIDI channel/Motor number being used. Index 0 is not used.
unsigned long motorSpeeds[] = {0, 0, 0, 0, 0}; //holds the speeds of the motors. 
unsigned long prevStepMicros[] = {0, 0, 0, 0, 0}; //last time
const bool motorDirection = LOW; //you can use this to change the motor direction, comment out if you aren't using it.
bool disableSteppers = HIGH; //status of the enable pin. disabled when HIGH. Gets enabled when the first note on message is received.
unsigned long WDT; //Will store the time that the last event occured.

MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI); //use default MIDI settings

MD_MAX72XX board = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


void setup() 
{
  pinMode(stepPin_M1, OUTPUT);
  pinMode(stepPin_M2, OUTPUT);
  pinMode(stepPin_M3, OUTPUT);
  pinMode(stepPin_M4, OUTPUT);
  pinMode(enPin, OUTPUT);

  MIDI.begin(MIDI_CHANNEL_OMNI); //listen to all MIDI channels
  MIDI.setHandleNoteOn(handleNoteOn); //execute function when note on message is recieved
  MIDI.setHandleNoteOff(handleNoteOff); //execute function when note off message is recieved
  board.begin();  
   for(int i = 31; i >=0; i--){
     board.setPoint(0,i,true);
  }
  Serial.begin(9600);
}

void loop() 
{
  MIDI.read(); //read MIDI messages
  digitalWrite(enPin, disableSteppers); //choose whether to enable or disable steppers.
  singleStep(1, stepPin_M1); //run each stepper at specified speed
  singleStep(2, stepPin_M2);
  singleStep(3, stepPin_M3);
  singleStep(4, stepPin_M4);
  if (millis() - WDT >= TIMEOUT)
  {
    disableSteppers = HIGH; //When the time has elapsed, disable the steppers
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity) //MIDI Note ON Command
{
  disableSteppers = LOW; //enable steppers. 
  motorSpeeds[channel] = pitchVals[pitch]; //set the motor speed to specified pitch
  /*
   * something that you could potentially do is have a grid of steppers
   * and use the velocity value to determine the number of steppers in each
   * collumn should be turned on. Each collumn would have its own MIDI channel.
   * The floppotron essentially does this...
   */ 
   drawLine(pitch, velocity/12 - 1, true);
   for(int i = 31; i >=0; i--){
     board.setPoint(0,i,true);
   }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) //MIDI Note OFF Command
{
  motorSpeeds[channel] = 0; //set motor speed to zero
  for(int i = 31; i >=0; i--){
     board.setPoint(0,i,true);
   }
   drawLine(pitch, 127, false);
}

void singleStep(byte motorNum, byte stepPin)
{
  if ((micros() - prevStepMicros[motorNum] >= motorSpeeds[motorNum]) && (motorSpeeds[motorNum] != 0)) 
  { //step when correct time has passed and the motor is at a nonzero speed
    prevStepMicros[motorNum] += motorSpeeds[motorNum];
    WDT = millis(); //update watchdog timer
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
  }
}

void drawLine(int pitch, int height, boolean onoff){
  int col = 0;
  if (pitch > 10 && pitch < 22) {
    col = 1;
  }
  if (pitch > 22 && pitch < 23) {
    col = 2;
  }
  if (pitch > 23 && pitch < 24) {
    col = 3;
  }
  if (pitch > 24 && pitch < 25) {
    col = 4;
  }
  if (pitch > 25 && pitch < 27) {
    col = 5;
  }
  if (pitch > 27 && pitch < 29) {
    col = 6;
  }
  if (pitch > 29 && pitch < 31) {
    col = 7;
  }
  if (pitch > 31 && pitch < 33) {
    col = 8;
  }
  if (pitch > 33 && pitch < 35) {
    col = 9;
  }
  if (pitch > 35 && pitch < 38)  {
    col = 10;
  }
  if (pitch > 38 && pitch < 39) {
    col = 11;
  } 
  if (pitch > 39 && pitch < 40) {
    col = 12;
  } 
  if (pitch > 40 && pitch < 41) {
    col = 13;
  } 
  if (pitch > 41 && pitch < 42) {
    col = 14;
  } 
  if (pitch > 42 && pitch < 43) {
    col = 15;
  } 
  if (pitch > 43 && pitch < 44) {
    col = 16;
  } 
  if (pitch > 45 && pitch < 47) {
    col = 17;
  } 
  if (pitch > 47 && pitch < 49) {
    col = 18;
  } 
  if (pitch > 49 && pitch < 51) {
    col = 19;
  } 
  if (pitch > 51 && pitch < 53) {
    col = 20;
  } 
  if (pitch > 53 && pitch < 55) {
    col = 21;
  } 
  if (pitch > 55 && pitch < 57) {
    col = 22;
  } 
  if (pitch > 57 && pitch < 59) {
    col = 23;
  }
  if (pitch > 59 && pitch < 61) {
    col = 24;
  }
  if (pitch > 61 && pitch < 63) {
    col = 25;
  }
  if (pitch > 63 && pitch < 65) {
    col = 26;
  }
  if (pitch > 65 && pitch < 67) {
    col = 27;
  }
  if (pitch > 67 && pitch < 69) {
    col = 28;
  }
  if (pitch > 69 && pitch < 71) {
    col = 29;
  }
  if (pitch > 71 && pitch < 72) {
    col = 30;
  } 
  if (pitch > 72 && pitch < 112) {
    col = 31;
  } 
  for(int i = 0; i < height; i++){
    board.setPoint(i, col, onoff);
  }
  for(int i = 0; i < height - 2; i++){
    board.setPoint(i , col + 1, onoff);
  }
  for(int i = 0; i < height - 2; i++){
    board.setPoint(i, col - 1, onoff);
  }
  for(int i = 0; i < height - 4; i++){
    board.setPoint(i, col - 2, onoff);
  }
   for(int i = 0; i < height - 4; i++){
    board.setPoint(i, col + 2, onoff);
  }
  for(int i = 0; i < height - 6; i++){
    board.setPoint(i, col - 3, onoff);
  }
  for(int i = 0; i < height - 6; i++){
    board.setPoint(i, col + 3, onoff);
  }
  for(int i = 31; i >=0; i--){
     board.setPoint(0,i,true);
  }
}
