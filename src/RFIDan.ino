#include "RFIDan.h"
#include "clicli.h"

RFIDan myrfid(5);
clicli mycli; 

// pins: 
//

String Tag1 = "D3DF2CAD"; //create a variable for your tag
String Tag2 = "534C9F1"; //create a variable for another tag
String Tag3 = "296A65A3"; //Create a variable for another tag

void setup() {
  myrfid.begin(); //initiate RFIDan
  mycli.begin(); //initiate clicli
}

void loop() {
  mycli.run(); //start clicli
  myrfid.check(Tag1, Tag2, Tag3, "Dan"); //checks for Tag1 and Tag2 and if finds one of them enables "access"
  //tone(3, 1000);
  //myrfid.printlcd(1, 1);
  //myrfid.allpix(0, 255, 0);
}