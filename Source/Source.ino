//subsystems: 
//1. bare arduino able to generate accurate Totps with secret key and time
//2. integration of keypad and typing, "enter"
//3. integration of lcd screen and print outputs/status
//4. integration of hardware real time clock so that arduino can sync time even when powered off
//5. integration of keypad inputs and comparison with the correct otp
//6. integration of useful outputs upon success, eg servo lock actuation or led bulb lights

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
  rtc.setTime(03, 20, 50); //hh mm ss , gmt time
  
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
