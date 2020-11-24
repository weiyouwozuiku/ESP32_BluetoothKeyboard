#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <Wire.h>     
#include <BleKeyboard.h>
#include <Adafruit_Fingerprint.h>
#define mySerial Serial2 
BleKeyboard bleKeyboard;
SSD1306Wire display(0x3c, 21, 22);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial); 
void displayHello(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawStringMaxWidth(0, 0, 128,
      "Weclome to use King's fingerprint bluetooth keyboard!" );
  display.display();
}
void displayMeg(String word){
    display.clear();
    display.drawStringMaxWidth(0,0,128,word);
    display.display();
    delay(2000);
}
void connectFinger(){
  if (finger.verifyPassword()) {
    displayMeg("Found fingerprint sensor! :)");
  } else {
    display.clear();
    //Serial.println("Did't find fingerprint sensor :(");
    display.drawStringMaxWidth(0,0,128,"Did't find fingerprint sensor :(");
    display.display();
    delay(2000);
    while (1) { 
      delay(2000);
      display.clear();
      //Serial.println("waiting for fingerprint sensor...");
      display.drawStringMaxWidth(0,0,128,"waiting for fingerprint sensor..."); 
      display.display();
      }
  }
}
void getValid(){
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    displayMeg("Sensor doesn't contain any fingerprint data. Please run the 'enroll'.");
  }
  else {
    displayMeg("Waiting for valid finger...");
    displayMeg("Sensor contains templates");
  }
}

void sendPassword(uint8_t id){
    display.clear();
    char stringId[25];
    itoa(id,stringId,10);
    display.drawString(0,0,"Fingerprint id :");
    display.drawString(0,16,stringId);
    display.display();
    if(bleKeyboard.isConnected()) {
      bleKeyboard.print("helloworld");//自定义密码
      bleKeyboard.write(KEY_RETURN);
    }
    display.drawString(0,32,"Key sended by BT!");
    display.display();
    delay(3000);
}
uint8_t getFingerprintID(){
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }

  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  sendPassword(finger.fingerID);
  return finger.fingerID;
}
void setup() {
  //Serial.begin(115200);
  //Serial.println("Weclome to use King's fingerprint bluetooth keyboard!");
  display.init();
  display.flipScreenVertically();
  displayHello();
  bleKeyboard.begin();
  finger.begin(57600);
  connectFinger();
  getValid();
}

void loop() {
  displayMeg("Waiting your finger,king!");
  getFingerprintID();
}
