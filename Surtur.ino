#include <OneWire.h>
#include <DallasTemperature.h>

//Pins the Thermometers are using:
OneWire ds18x20[] = {49,50,51,52,53};
const int oneWireCount = sizeof(ds18x20)/sizeof(OneWire);
DallasTemperature sensor[oneWireCount];

void setup(void) {
  // start serial port
  Serial.begin(9600);
  
  //=======Thermometer SetUp=======
  
  DeviceAddress deviceAddress;
  for (int i = 0; i < oneWireCount; i++) {;
    sensor[i].setOneWire(&ds18x20[i]);
    sensor[i].begin();
    if (sensor[i].getAddress(deviceAddress, 0)) sensor[i].setResolution(deviceAddress, 12);
  }

  //LCD Set Up
  
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
void loop(void) 
{
  Serial.println("");
  float tmp = GetAverageTemperature();
  Serial.print("average temp: ");
  Serial.println(tmp);
}
