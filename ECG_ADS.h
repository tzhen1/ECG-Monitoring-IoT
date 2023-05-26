#include "Arduino.h"
#include "LPF.h"
//#define PLOTT_DATA //commented out so not print data
#define MAX_BUFFER 100
#define BUZZER 12 // buzzer

uint32_t prevData[MAX_BUFFER];
uint32_t sum=0;
uint32_t maxVal=0;
uint32_t avgVal=0;
uint32_t index_count=0;
uint32_t countVal=0;
uint32_t period=0;
uint32_t lastperiod=0;
uint32_t millistimer = millis();
float frequency;
float beatspermin=0;
int capture_heart_abnormal = 0;
uint32_t newVal; // ecg value

// ECG LPF
int ECG_val_1 = 0;
int ECG_val_filtered = 0;
BW_Filter lpf;
// ECG 1000 data points, 5 second sample period
int ECG_val_output[1000] = { 0 };

void setup_ECG() 
{
  //serial communication
  Serial.begin(115200);
    
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
}

void loop_ECG() 
{
  for (int i = 0; i <= 999; i++) 
  {
    ECG_val_1 = analogRead(A0);
    ECG_val_output[i] = ECG_val_1; //ECG_val_1

    // BW LPF
    //ECG_val_output[i] = BW_Filter.step(ECG_val_1); 
    delay(4);                                       
  }

  for (int i = 0; i <= 999; i++) 
  {
    Serial.print(ECG_val_output[i]);
    Serial.print(',');
  }
  Serial.print("\n");
 
  delay(10); //Wait to keep serial from saturating
}

void freqDetect() 
{
  if (countVal==MAX_BUFFER) 
  { // preData = ecg value
    if (prevData[index_count] < avgVal * 1.5 && newVal >= avgVal * 1.5)
    { 
      period = millis() - millistimer; //get period from current timer value
      millistimer = millis();//reset timer
      maxVal = 0;
    }
  }
  index_count++;

  if (index_count >= MAX_BUFFER) { index_count=0;} // reset counter

  if (countVal<MAX_BUFFER) 
  {
    countVal++; // count up to max
    sum += newVal; // sum of ecg values
  } 
  else 
  {
    sum += newVal - prevData[index_count];
  }
  avgVal = sum/countVal; // sum of samples to number of samples

  if (newVal>maxVal) { maxVal = newVal;} // max val if ecg peak

  #ifdef PLOTT_DATA
  Serial.print(newVal);
  Serial.print("\t");
  Serial.print(avgVal);
  Serial.print("\t");
  Serial.print(avgVal*1.5);
  Serial.print("\t");
  Serial.print(maxVal);
  Serial.print("\t");
  Serial.println(beatspermin);
  #endif

  prevData[index_count] = newVal;//store previous value

}

void loop_ECG_HR() 
{
  newVal = analogRead(A0);
  freqDetect(); // func, get frequency

  if (period!=lastperiod) 
  {
    frequency = 1000/(float)period;//timer (ms) rate/period (ms)

    if ( (frequency * 60) > 60 && (frequency * 60) < 100) // make valid, from 20 to 200 before
    { 
      beatspermin = frequency*60;

      #ifndef PLOTT_DATA
      //Serial.print(frequency);
      //Serial.print(" hz");
      //Serial.print(" ");
      Serial.print(beatspermin);
      Serial.println(" bpm");
      #endif
      lastperiod=period;
      
    }
    else if(capture_heart_abnormal == 0)
    {
          // buzz 
      digitalWrite(BUZZER, HIGH);
      delay(10);
      digitalWrite(BUZZER, LOW);
      capture_heart_abnormal = 1;

    }
  }
  delay(5);
}
