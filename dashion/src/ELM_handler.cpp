#include "ELM_handler.h"

ELM327_Handler::ELM327_Handler(BluetoothSerial &btSerial) : _bt(btSerial) {
    // _bt means SerialBT in main file
}

/**
 * @brief connect to ELM327 and prepare AT comms.
 */
bool ELM327_Handler::begin(const char *btNameOrAddress, bool isAddress) {
    


    // start BT as a "master"
    if (!_bt.begin("ESP32_Master", true)) {
        Serial.println("Error: Bluetooth Master mode cannot initialized!");
        return false;
    }

    _bt.setPin("1234");                                 // setting PIN code if ELM327 requests it,
                                                        // pin code "1234", "0000", "7890", "1111" as I search
    Serial.println("PIN code setted up as '1234' ..."); // change this one too if needed


    // connect with address or name
    bool connected = false;
    if (isAddress) {
        // must convert string to uint8_t[6] 
        // connected = _bt.connect(address_bytes); 
        Serial.println("connection with address couldn't completed.");
        return false; 
    } else {
        Serial.print(btNameOrAddress);
        Serial.println(" (name) connecting...");
        connected = _bt.connect(btNameOrAddress);
    }

    if (!connected) {
        Serial.println("ELM327 connection failed!");
        return false;
    }

    Serial.println("ELM327 connected, preparing device...");

    // ELM327 AT comm init
    if (!sendCommand("ATZ")) { // reset device
        Serial.println("ATZ (Reset) command failed.");
        return false;
    }
    delay(1000); // wait for reset

    if (!sendCommand("ATE0")) { // disable echo
        Serial.println("ATE0 (Echo Off) command failed.");
        // non-must command
    }
    
    if (!sendCommand("ATL0")) { // disable linefeed

    }

    Serial.println("ELM327 is ready to mission.");
    return true;
}

/**
 * @brief fetchs the engine RPM value
 * @return int RPM value, -1 if error occurs
 */
int ELM327_Handler::getRPM() {
    if (sendCommand("010C")) { // PID "010C" = Engine RPM
        // response format: "41 0C AA BB"
        // RPM formula: (AA * 256 + BB) / 4
        return parseResponse("010C", 2, "RPM");
    }
    return -1; // sending comm failed
}

/**
 * @brief sends raw command (adds '\r' at the end)
 */
bool ELM327_Handler::sendCommand(const char *cmd) {
    Serial.print("sent command: ");
    Serial.println(cmd);

    _bt.println(cmd); // send command to bluetooth
    _bt.flush();
    return true;
}

/**
 * @brief reads the response from ELM327 and stores it into buffer
 * response ends with '>' char.
 */
bool ELM327_Handler::readResponse(unsigned long timeout) {
    _responseBuffer = ""; // clean buffer
    char incomingChar;
    unsigned long startTime = millis();

    while (millis() - startTime < timeout) {
        if (_bt.available()) {
            incomingChar = _bt.read();
            if (incomingChar == '>') { // if prompt arrives, it means response is done
                Serial.print("Response: ");
                Serial.println(_responseBuffer);
                return true;
            }
            // skip the echos and spaces
            if (incomingChar != '\r' && incomingChar != '\n' && incomingChar != ' ') {
                _responseBuffer += incomingChar;
            }
        }
    }
    Serial.println("Error: Response timed out!");
    return false;
}

/**
 * @brief parses the raw value data
 */
int ELM327_Handler::parseResponse(const char *pid, int bytesToRead, const char *formula) {
    if (!readResponse()) { // read first
        return -1; // no answer
    }

    // check if "41" exists (success command) and any PID available (eg. "0C")
    String expectedHeader = "41" + String(pid); // "410C"
    
    if (_responseBuffer.startsWith(expectedHeader)) {
        // skip "410C" (4 chars)
        String dataHex = _responseBuffer.substring(expectedHeader.length());
        
        // data bytes
        unsigned long valA = 0;
        unsigned long valB = 0;

        if (bytesToRead == 1) {
            valA = strtoul(dataHex.substring(0, 2).c_str(), NULL, 16);
        } else if (bytesToRead == 2) {
            valA = strtoul(dataHex.substring(0, 2).c_str(), NULL, 16); // "1A"
            valB = strtoul(dataHex.substring(2, 4).c_str(), NULL, 16); // "F0"
        }

        // apply the formula (for RPM)
        if (strcmp(formula, "RPM") == 0) {
            // RPM formula: (A * 256 + B) / 4
            return ((valA * 256) + valB) / 4;
        }
        // other PID's will be added
        // ...
    }
    
    Serial.println("Error: Invalid answer type!");
    return -1; // response didn't start with "41 0C" 
}