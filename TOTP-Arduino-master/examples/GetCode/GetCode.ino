// GetCode.ino
// 
// Basic example for the TOTP library
//
// This example uses the opensource SwRTC library as a software real-time clock
// you can download from https://github.com/leomil72/swRTC
// for real implementation it's suggested the use of an hardware RTC

#include "sha1.h"
#include "TOTP.h"
#include "swRTC.h"

// The shared secret is MyLegoDoor// SecretKey1
uint8_t hmacKey[] = {0x53, 0x65, 0x63, 0x72, 0x65, 0x74, 0x4b, 0x65, 0x79, 0x31}; //{0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};

TOTP totp = TOTP(hmacKey, 10);
swRTC rtc;
char code[7];


void setup() {
  
  Serial.begin(9600);
  rtc.stopRTC();
  
  // Adjust the following values to match the current date and time
  // and power on Arduino at the time set (use GMT timezone!)
  rtc.setDate(17, 9, 2016); //dd m yyyy
  rtc.setTime(07, 30, 50); //hh mm ss , gmt time
  
  rtc.startRTC();
}

void loop() {

  long GMT = rtc.getTimestamp();
  char* newCode = totp.getCode(GMT);
  if(strcmp(code, newCode) != 0) {
    strcpy(code, newCode);
    Serial.print("Your OTP code is: ");
    Serial.println(code);
  }  
}
