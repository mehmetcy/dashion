
#include <Arduino.h>
#include "BluetoothSerial.h"

// function declarations below;
void ledBlink1s(void);
void ledBlink250ms(void);

#define LED 2

BluetoothSerial SerialBT;

const char *bt_address_str = "PC:MA:CA:DD:RE:SS"; // bt address here smth like AA:11:BB:22:CC:33
const char *bt_name = "DESKTOP-YOURDESKTOPNAME";  // pc name here smth like DESKTOP-BLA1BLA2BLA3
uint8_t bt_address[6];

// *********************************************************************  ----------> you can copy+paste as a bracket =)

//  converting function string -> byte array
void parseAddress(const char *str, uint8_t *addr) 
{
    sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
           &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5]);
}

void setup() 
{
  pinMode(LED, OUTPUT); // LED init, can be useful for debug flags

  Serial.begin(115200);
  Serial.println("*********************************************************************");
  Serial.println("bluetooth comm test starting...");
  Serial.println("*********************************************************************");

  parseAddress(bt_address_str, bt_address);

  SerialBT.begin("ESP32_master" ,true); // true means master mode
  Serial.println("*********************************************************************");
  Serial.println("bluetooth initialized as a master!");
  Serial.println("*********************************************************************");
  Serial.print("trying connection to the address (Address: ");
  Serial.print(bt_address_str);
  Serial.println(")...");

  bool btConnected = SerialBT.connect(bt_name); // SerialBT.connect(bt_address) can be optional..

  if(btConnected)
  {
    Serial.println("successfully connected! :) :)");
    ledBlink250ms();
  }
  else
  {
    Serial.println("connection failed! :( ");

    while (1)
    {
      ledBlink1s();
    }
    
  }

}

void loop() 
{
  if (SerialBT.available())
  {
    char incomingChar = SerialBT.read();
    Serial.print(incomingChar);
  }
  
  if (Serial.available())
  {
    char outgoingChar = Serial.read();
    SerialBT.write(outgoingChar);
  }
  
  

}

// function definitions below;
void ledBlink1s() 
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}

void ledBlink250ms() 
{
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
}

