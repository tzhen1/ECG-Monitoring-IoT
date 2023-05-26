#include <WiFi.h>
#include "ThingSpeak.h"

const char *ssid = "Huawei_phone";     // wifi ssid
const char *password = "wifi_pass";   // Wifi-password

WiFiClient client;
unsigned long myChannelNumber = chan_val;
const char * myWriteAPIKey = "aPII";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

float ecg_data [1000] = {0}; 
float spo2_data = 0;

void setup_cloud() 
{
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak

}

void loop_cloud(float beatspermin, int32_t spo2, float temp) 
{
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED)
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, password);
      Serial.print(".");
      delay(5000);     
    } 

    Serial.println("\nConnected to Wifi");
  }
  // set the fields with the values
  ThingSpeak.setField(1, beatspermin);
  ThingSpeak.setField(2, spo2);
  ThingSpeak.setField(3, temp);

  // write to cloud
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(x == 200)
  {
    Serial.println("Channel update successful.");
    delay(10);
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    delay(1000);
  }
  lastTime = millis();

  delay(2000);
}
