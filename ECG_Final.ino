#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include "MAX_Temp.h"
#include "spo2.h"
#include "ECG_ADS.h"
#include "bluetooth.h"
#include "ThingSpeak.h"
#include "cloud.h"

MAX30205 max30205;

// store ecg, spo2, temp in large array
int capture1 = 0;
//int capture2;

void setup()
{
  Serial.begin(115200);
  setup_AFE4400();
  setup_ECG();
  max30205.begin(0x48);
  setup_ble();

  // cloud setup
  // setup_cloud();

}

void loop()
{

  if (capture1 == 0)
  {
    capture1 = 1;
    
    Serial.print(max30205.readTemperature());
    Serial.println("C");
    loop_ECG();
  }

  if(capture2 == 0)
  {
    loop_AFE4400();
  }

  if(capture2 == 1)
  {
    loop_ble(ECG_val_output, spo2_prev, max30205.readTemperature()); 
  }

  /* testing cloud, enable cloud setup */
  // loop_ECG_HR();

  // // Serial.println(max30205.readTemperature());
  // // Serial.println("C");

  // if(capture2 == 0)
  // {
  //   loop_AFE4400();
  // }

  // if(capture2 == 1)
  // {
  //   loop_cloud(beatspermin, spo2_prev, max30205.readTemperature());
  // }


}