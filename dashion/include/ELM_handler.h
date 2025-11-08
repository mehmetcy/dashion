#pragma once

#include <Arduino.h>
#include <BluetoothSerial.h>

class ELM327_Handler {
public:

// constructer function
ELM327_Handler(BluetoothSerial &btSerial);

// connect to ELM327
bool begin(const char *btNameOrAddress, bool isAddress = false);

// catching parameter datas
int getRPM();
int getSpeed();
float getTurboBoost();          // will be calculated by ourself, because ELM won't provide it
                                // turbo boost = manifold absolute pressure - atmoshperic pressure

int getManifoldPressure();      // turbo intake raw value

private:
    BluetoothSerial &_bt;       // bt object referance
    String _responseBuffer;     // ELM327 value&data storage

    // included auxiliary functions
    bool sendCommand(const char *cmd);
    bool readResponse(unsigned long timeout = 1000);
    int parseResponse(const char *pid, int bytesToRead, const char *formula);

};