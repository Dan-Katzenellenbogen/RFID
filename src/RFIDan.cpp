#include "Arduino.h"
#include "RFIDan.h"
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include "clicli.h"
#include <string.h>

#define BUZZER_PIN 3
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
#define RST_PIN 9
#define SS_PIN 10
#define NUMPIXELS 8
#define PIN 5 
#define PIN2 6

int notes[] = {
  466, 466, 415, 415,   
  698, 698, 622,        
  466, 466, 415, 415,   
  622, 622, 554, 523, 466  
};

int durations[] = {
  180, 180, 180, 180,
  280, 280, 400,
  180, 180, 180, 180,
  280, 280, 280, 220, 350
};

LiquidCrystal_I2C lcd(0x27,16,2); 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
RFIDan::RFIDan(int RFID_pin) {
  _RFID_pin = RFID_pin;
}

void playRickroll();

byte readCard[4];
String tagID = ""; //a string variable to store the last tag's ID

boolean getID() //a function to that returns True if a tag is detected and also the ID of the Tag
{
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  // Getting ready for Reading PICCs
  //If a new PICC placed to RFID reader continue
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  //Since a PICC placed get Serial and continue
  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  tagID = "";
  // The MIFARE PICCs that we use have 4 byte UID
  for ( uint8_t i = 0; i < 4; i++) {
  //readCard[i] = mfrc522.uid.uidByte[i];
  // Adds the 4 bytes in a single String variable
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
}

void RFIDan::begin() {
  Serial.begin(9600);
  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();		// Init MFRC522
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  lcd.init();                   
  lcd.backlight();
  pixels2.setBrightness(3);
  pixels.setBrightness(3);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(_RFID_pin, OUTPUT);
  pixels.begin(); // initialize NeoPixel 
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();
  pixels2.begin();
  pixels2.clear();
  pixels2.show();
  lcd.setCursor(0, 0);
  lcd.print("Program Started");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Access Control");
}

void RFIDan::check(String Tag1,String Tag2,String Tag3,String Name){
  while(getID()){
      if (tagID == Tag1) { //checks if last detected tag is Tag1 and grants access if yes
        Serial.println("Access Granted");
        Serial.println("Hi " + Name);
        Serial.println("---------------------");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        lcd.setCursor(0, 1);
        lcd.print("Hi Dan!");
        tone(BUZZER_PIN, 1000, 200);
        for (int i = 7; i >= 0; i--){
          pixels.setPixelColor(i, 0, 255, 0);
          pixels2.setPixelColor(i, 0, 255, 0);
          if (i == 5)
          {
            tone(BUZZER_PIN, 1700, 200);
          }
          if (i == 3)
          {
            tone(BUZZER_PIN, 2500, 200);
          }
          if (i == 1)
          {
            tone(BUZZER_PIN, 2200, 100);
          }
          if (i == 0)
          {
            tone(BUZZER_PIN, 2500, 200);
          }
          pixels2.show();
          pixels.show();
          delay(100);
        }
      }
      else if(tagID == Tag2) { //checks if last detected tag is Tag2 and grants access if yes
        Serial.println("Access Granted");
        Serial.println("Hi " + Name);
        Serial.println("---------------------");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        lcd.setCursor(0, 1);
        lcd.print("Hi Dan!");
        tone(BUZZER_PIN, 1000, 350);
        for (int i = 0; i < NUMPIXELS; i++){
          pixels.setPixelColor(i, 0, 255, 0);
          pixels.show();
          pixels2.setPixelColor(i, 0, 255, 0);
          pixels2.show();
        }
      }
      else if(tagID == Tag3) {
        playRickroll();
      }
      else { //denies access when last detected tag is not Tag1 or Tag2
        Serial.println("Access Denied");
        Serial.println("---------------------");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        for (int i = 0; i < 5; i++){
          for (int i = 0; i < NUMPIXELS; i++){
            pixels2.clear();
            pixels2.show();
            pixels.clear();
            pixels.show();
            tone(BUZZER_PIN, 400, 100);
          }
          delay(100);
          for (int i = 0; i < NUMPIXELS; i++){
            pixels.setPixelColor(i, 255, 0, 0);
            pixels.show();
            pixels2.setPixelColor(i, 255, 0, 0);
            pixels2.show();
          }
          delay(100);
        }
        pixels.clear();
        pixels.show();
        pixels2.clear();
        pixels2.show();
      }
      delay(1500);
      Serial.println("Access Control"); //notifies when tags can be detected again
      Serial.println("---------------------");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Control");
      for (int i = 0; i < NUMPIXELS; i++){
        pixels2.clear();
        pixels2.show();
        pixels.clear();
        pixels.show();
      }
  }
}

void RFIDan::printlcd(bool access, int ID){
  if (access){
    lcd.setCursor(0, 5);
    lcd.print("Access granted");
    if (ID == 1){
      lcd.setCursor(1, 2);
      lcd.print("Welcome, John");
    } else if (ID == 2){
        lcd.setCursor(1, 2);
        lcd.print("Welcome, Ruth"); 
    } else{
        lcd.setCursor(1, 2);
        lcd.print("Welcome, Stranger");
    }
  } else{
      lcd.setCursor(0, 5);
      lcd.print("Access Denied!");
  }
}

void RFIDan::neo(int pixno, int r, int g, int b){
  pixels.setPixelColor(pixno, pixels.Color(r, g, b));
  pixels2.setPixelColor(pixno, pixels2.Color(r, g, b));
  pixels.show();   // Send the updated pixel colors to the hardware.
  pixels2.show();
}

void RFIDan::allpix(int r, int g, int b){
  for (int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, r, g, b);
    pixels2.setPixelColor(i, r, g, b);
  }
  pixels2.show();
  pixels.show();
}

uint32_t colors[] = {
  pixels.Color(255, 0, 0),
  pixels.Color(0, 255, 0),
  pixels.Color(0, 0, 255),
  pixels.Color(255, 255, 0),
  pixels.Color(0, 255, 255),
  pixels.Color(255, 0, 255),
  pixels.Color(255, 128, 0),
  pixels.Color(128, 0, 255)
};

void clearPixels() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
    pixels2.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
  pixels2.show();
}

void playRickroll() {
  pixels.begin();
  pixels2.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Get rickrolled");
  
  int colorIndex = 0;

  for (int i = 0; i < sizeof(notes) / sizeof(int); i++) {
    int note = notes[i];
    int duration = durations[i];

    tone(BUZZER_PIN, note, duration);

    // Choose a color and increment the index
    uint32_t color = colors[colorIndex];
    colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(uint32_t));

    clearPixels(); // Start clean

    // Determine how many pixels to light based on intensity of the note
    int pixelCount = map(note, 400, 700, 4, 8); // range: 4–8 per strip (8–16 total)

    for (int j = 0; j < pixelCount; j++) {
      // Reverse the index direction for each strip
      pixels.setPixelColor(NUMPIXELS - 1 - j, color);
      pixels2.setPixelColor(NUMPIXELS - 1 - j, color);
    }

    pixels.show();
    pixels2.show();

    delay(duration * 1.3);

    clearPixels(); // Fade to black between notes
  }
}
