#include <SPI.h>
#include "AFE4400.h"
#define BUZZER 12 // buzzer
#define LED_GREEN 2 // buzzer

#define AFE4400_CS_PIN 21
#define AFE4400_PWDN_PIN 4
#define AFE44XX_INTNUM 0

AFE44XX afe44xx(AFE4400_CS_PIN, AFE4400_PWDN_PIN);  // calcualtes and outputs to serial monitor

afe44xx_data afe44xx_raw_data;
int32_t heart_rate_prev = 0;
int32_t spo2_prev = 0;
int32_t spo2_default = 0;
int capture2 = 0;

void spo2_change()
{
  spo2_prev = random(95, 98);
  delay(10);
}

void setup_AFE4400() 
{
  Serial.begin(115200);
  //Serial.println("Intilaziting AFE44xx.. ");

  SPI.begin();
  afe44xx.afe44xx_init();
  Serial.println("Initiated SPO2");

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);

}


void loop_AFE4400() 
{
  delay(8);

  afe44xx.get_AFE44XX_Data(&afe44xx_raw_data);
  //Serial.println(spo2_prev);
  //spo2_prev= 95;

  if (afe44xx_raw_data.buffer_count_overflow) // overflow = true
  {
    if (afe44xx_raw_data.spo2 == -999)  //
    {
      Serial.println("spo2!");
      // spo2_change();
      //Serial.println(spo2_prev);
      //delay(5000);
    } 

    // if (afe44xx_raw_data.spo2 > 0 && afe44xx_raw_data.spo2 < 90 && afe44xx_raw_data.spo2 != -999)  //
    // {
    //   Serial.println("spo2 below 90%");
    //   digitalWrite(BUZZER, HIGH);
    //   delay(100);
    //   digitalWrite(BUZZER, LOW);
    // } 

    else if ((heart_rate_prev != afe44xx_raw_data.heart_rate) || (spo2_prev != afe44xx_raw_data.spo2)) // a change from -999
    {
      heart_rate_prev = afe44xx_raw_data.heart_rate;
      spo2_prev = afe44xx_raw_data.spo2;

      Serial.print("\n SP02: ");
      Serial.print(afe44xx_raw_data.spo2);
      Serial.print("% \n");
      capture2 = 1;

      // buzz 
      digitalWrite(LED_GREEN, HIGH);
      // delay(50);
      // digitalWrite(LED_GREEN, LOW);
    }
  }
}