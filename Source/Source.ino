//subsystems: 
//1. DONE bare arduino able to generate accurate Totps with secret key and time
//2. integration of keypad and typing, "enter"
//3. integration of lcd screen and print outputs/status
//4. DONE integration of hardware real time clock so that arduino can sync time even when powered off
//5. integration of keypad inputs and comparison with the correct otp
//6. integration of useful outputs upon success, eg servo lock actuation or led bulb lights

#include "sha1.h"
#include "TOTP.h"
//#include "swRTC.h"

#include "Time.h"
#include "DS1302RTC.h"

// The shared secret is MyLegoDoor// SecretKey1
uint8_t hmacKey[] = {0x53, 0x65, 0x63, 0x72, 0x65, 0x74, 0x4b, 0x65, 0x79, 0x31}; //{0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};

TOTP totp = TOTP(hmacKey, 10);
//swRTC rtc;
char code[7];

//RTC Set pins:  CE, IO,CLK
DS1302RTC RTC(5, 6, 7);


void setup() {
  
  Serial.begin(9600);
//  rtc.stopRTC();
//  
//  // Adjust the following values to match the current date and time
//  // and power on Arduino at the time set (use GMT timezone!)
//  rtc.setDate(17, 9, 2016); //dd m yyyy
//  rtc.setTime(04, 44, 30); //hh mm ss , gmt time
//  
//  rtc.startRTC();
}

void loop() {

  //long GMT = rtc.getTimestamp(); //this is for the softRTC internal to arduino

  //Serial.print("UNIX Time: ");
  //Serial.print(RTC.get());
  long GMT = RTC.get(); //this is to get the time from the hardware RTC
  char* newCode = totp.getCode(GMT);
  if(strcmp(code, newCode) != 0) {
    strcpy(code, newCode);
    //Serial.println(rtc.getTimestamp()); //to print soft time
    //Serial.println(GMT);
    Serial.print("Your OTP code is: ");
    Serial.println(code); //prints the code

    tmElements_t tm;
    if (! RTC.read(tm)) {
    Serial.println("Valid for:"); //prints the time
    Serial.print("  Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.print(", DoW = ");
    Serial.print(tm.Wday);
    Serial.println();
    }
  }  
}

void print2digits(int number) {
  if (number >= 0 && number < 10)
    Serial.write('0');
  Serial.print(number);
}
