#ifndef RFIDAN_H
#define RFIDAN_H

//#include "RFIDan.h"
#include "Arduino.h"

 class RFIDan {
  private:
  int _RFID_pin;
  
  
  public: 
   RFIDan(int RFID_pin);
   void begin();
   void check(String Tag1, String Tag2, String Tag3, String Name);
   void printlcd(bool access, int ID);
   void neo(int pixno, int r, int g, int b);
   void allpix(int r, int g, int b);
 };
#endif 
