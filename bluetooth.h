#include "BluetoothSerial.h"
#define LED_1 2

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
 
BluetoothSerial SerialBT;
bool BT_connect = false; 

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if(event == ESP_SPP_SRV_OPEN_EVT)
  {
    Serial.println("BT Connected");
    BT_connect = true;
  }
 
  if(event == ESP_SPP_CLOSE_EVT )
  {
    Serial.println("BT disconnected");
    BT_connect = false;
    ESP.restart();
  }
}
  
void setup_ble()
{
  // inti serial
  Serial.begin(115200);

  // init ble
  SerialBT.register_callback(callback);
  if(!SerialBT.begin("ESP32_ECG")) // name of bluetooth
  {
    Serial.println("An error occurred initializing Bluetooth");
  }
  else
  {
    Serial.println("Bluetooth Device is Ready to Pair...");
  }

  delay(1000);
}
  
void loop_ble(int ECG_VAL [], int32_t spo2, float temp)
{
  if (BT_connect)
  { 
    for(int i = 0; i <= 999; i++)
    {
            // EA,B,C\n
      SerialBT.print("E");
      SerialBT.print(ECG_VAL[i]);
      //SerialBT.print(analogRead(A0));
      SerialBT.print(",");
      SerialBT.print(spo2);
      SerialBT.print(",");
      SerialBT.print(temp);
      SerialBT.print("\n");
    }

    delay(10);
  }

  delay(10);
}
