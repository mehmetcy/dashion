
#include <Arduino.h>
#include "BluetoothSerial.h"
#include "ELM_handler.h"


// function declarations below;
void ledBlink1s(void);
void ledBlink250ms(void);

#define LED 2

const char *ELM327_BT_NAME = "OBDII";  // name for ELM327 (usually OBDII or obd2 as I search it on forums..)
                                       // pin code "1234", "0000", "7890", "1111" as I search again...                                      

BluetoothSerial SerialBT;

ELM327_Handler elm(SerialBT);

// *********************************************************************  ----------> you can copy+paste as a bracket =)


void setup() {
    Serial.begin(115200);
    Serial.println("starting ELM327 OBD connection...");

    // connect to ELM327
    // (connecting with name)
    if (elm.begin(ELM327_BT_NAME, false)) { 
        Serial.println("successfully connected.");
    } else {
        Serial.println("initialization or connection failed.");
        while (1) {
            delay(1000); // stay here if error occurs
        }
    }
}

void loop() {
    
    int currentRPM = elm.getRPM();

    if (currentRPM != -1)     // if no error
    { 
        Serial.print("Engine RPM: ");
        Serial.println(currentRPM);
    } 
    else 
    {
        Serial.println("couldn't read RPM data!");
    }

    delay(250);               // data update rate 250ms
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

