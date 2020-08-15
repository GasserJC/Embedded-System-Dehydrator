#include <OneWire.h>
#include <DallasTemperature.h>
#include <string.h>
#include <stdio.h>
//Pins the Thermometers are using:
OneWire ds18x20[] = {50,51,52,53,54};
const int oneWireCount = sizeof(ds18x20)/sizeof(OneWire);
DallasTemperature sensor[oneWireCount];
//Pins the LCD is using
const int rs = 40, en = 41, d4 = 42, d5 = 43, d6 = 44, d7 = 45;
//Time displayed on LED numbers
static int time = 0;
const int buttonPin = 49;
const int ledPin0 = 48;
const int ledPin1 = 47;
const int ledPin2 = 46;
int buttonState = LOW;
int lastState = LOW;
static int _time = 0;
static unsigned long Start_Time = millis();
//Heater Variables
static bool setHeat = false;
const int transistorPin = 45;
void setup(void) {
  // start serial port
  Serial.begin(9600);
  //=======Thermometer Set Up=======
  DeviceAddress deviceAddress;
  for (int i = 0; i < oneWireCount; i++) {;
    sensor[i].setOneWire(&ds18x20[i]);
    sensor[i].begin();
    if (sensor[i].getAddress(deviceAddress, 0)) sensor[i].setResolution(deviceAddress, 12);
  }
//=======LED Set Up========
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin, INPUT);
//======Heater Set Up=====
pinMode(transistorPin, OUTPUT);
}
float GetAverageTemperature()
{
   Serial.print("Requesting temperatures...");
  for (int i = 0; i < oneWireCount; i++) {
    sensor[i].requestTemperatures();
  }
  delay(100);
  float temperature = 0;
  Serial.print( "Thermometer Temps: " );
  for (int i = 0; i < oneWireCount; i++) {
    //get and convert temp to freedom units
    Serial.print( i+1 ); Serial.print( "-T:" ); Serial.print( sensor[i].getTempCByIndex(0)*9/5 + 32 ); Serial.print(" ");
    temperature += sensor[i].getTempCByIndex(0)*9/5 + 32;
  }
  //average
  Serial.println("");
  temperature = (temperature)/(oneWireCount);
  return temperature;
}
//Sets time in hours across three LED's to represent time left in binary on the dehydrator
void updateTime(){
  if(_time <=6){ //max time is 7 hours
    _time++;
  }
  else{
    _time = 0;
  }
  if(_time%2 ){
    digitalWrite(ledPin0, LOW);
  }
  else{
    digitalWrite(ledPin0, HIGH);
  }
  if( _time%4  <= 2 ){
    digitalWrite(ledPin1, LOW);
  }
  else{
    digitalWrite(ledPin1, HIGH);
  }
  if( _time < 3 ){
    digitalWrite(ledPin2, LOW);
  }
  else{
    digitalWrite(ledPin2, HIGH);
  }
}

void applyHeat(){
  if(setHeat)
  {
    digitalWrite(transistorPin, HIGH);
  }
  else
  {
    digitalWrite(transistorPin, LOW);
  }
}

void loop(void) 
{
  lastState = LOW;
  buttonState = digitalRead(buttonPin);
  if(lastState != buttonState){
    lastState = buttonState;
    updateTime();
  }
  Serial.println("");
  float tmp = GetAverageTemperature();
  Serial.print("average temp: ");
  Serial.println(tmp);
  if(Start_Time >= _time*3600*1000 || GetAverageTemperature() > 155){
     setHeat = false;
  }
  else{
    setHeat = true;
  }
  applyHeat();
  delay(200);
}
