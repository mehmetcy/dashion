#include <HardwareSerial.h>


#define RXD2 16
#define TXD2 17


int valBoost = 0;  
int valOil = 70;    
int valLoad = 0;    
int valPedal = 0;   

int dirBoost = 1;
int dirOil = 1;
int dirLoad = 1;
int dirPedal = 1;

void setup() {

  Serial.begin(115200);


  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  delay(1000); 
  Serial.println("dashion simulation initializing...");
}

void loop() {

  valBoost += 1 * dirBoost; 
  if (valBoost >= 25) dirBoost = -1; 
  if (valBoost <= 0)  dirBoost = 1;  


  valOil += 1 * dirOil;
  if (valOil >= 120) dirOil = -1;
  if (valOil <= 70)  dirOil = 1;


  valLoad += 2 * dirLoad;
  if (valLoad >= 100 || valLoad <= 0) dirLoad *= -1;


  valPedal += 2 * dirPedal;
  if (valPedal >= 100 || valPedal <=0) dirPedal *= -1;


  sendNextion("va_boost", valBoost);
  sendNextion("va_oiltmp", valOil);
  sendNextion("va_engload", valLoad);
  sendNextion("va_accpdl", valPedal);

  Serial.print("Boost: "); Serial.print(valBoost);
  Serial.print(" | Oil: "); Serial.print(valOil);
  Serial.println(" -> sent to nextion");

  delay(100); 
}

void sendNextion(String degiskenAdi, int deger) {
  Serial2.print(degiskenAdi);
  Serial2.print(".val=");
  Serial2.print(deger);
  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}