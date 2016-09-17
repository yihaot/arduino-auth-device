//subsystems:
//1. DONE bare arduino able to generate accurate Totps with secret key and time
//2. DONE integration of keypad and typing, "enter"
//3. DONE integration of lcd screen and print outputs/status
//4. DONE integration of hardware real time clock so that arduino can sync time even when powered off
//5. DONE integration of keypad inputs and comparison with the correct otp
//6. DONE integration of useful outputs upon success, eg servo lock actuation or led bulb lights

#include "sha1.h"
#include "TOTP.h"
//#include "swRTC.h"

#include "Time.h"
#include "DS1302RTC.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"

#include "Keypad.h"

// The shared secret is MyLegoDoor// SecretKey1
uint8_t hmacKey[] = {0x53, 0x65, 0x63, 0x72, 0x65, 0x74, 0x4b, 0x65, 0x79, 0x31}; //{0x4d, 0x79, 0x4c, 0x65, 0x67, 0x6f, 0x44, 0x6f, 0x6f, 0x72};

TOTP totp = TOTP(hmacKey, 10);
//swRTC rtc;
char code[7];

//RTC Set pins:  CE, IO,CLK
DS1302RTC RTC(10, 11, 12);

//I2C LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//keypad
const byte numRows = 4; //number of rows on the keypad
const byte numCols = 4; //number of columns on the keypad
//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9, 8, 7, 6}; //Rows 0 to 3
byte colPins[numCols] = {5, 4, 3, 2}; //Columns 0 to 3
//initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);



//variables
String keyEntered = "";

void setup() {
  //LED Output
  analogWrite(A3, 0); //gnd of led
  digitalWrite(13, HIGH); //vcc for hw rtc

  Serial.begin(9600);

  lcd.begin(16, 2);

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

  //keypad stuff
  char keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY)
  {
    Serial.print("keypressed is: ");
    Serial.println(keypressed);
    if (keypressed == char('*')) { //clear
      Serial.println("input cleared");
      //keyEntered = keyEntered.substring(0, keyEntered.length() - 1); //backspace
      keyEntered = "";
    }
    else if (keypressed == char('#')) {
      //enter command
      codeChecker(keyEntered);
      keyEntered = "";
    }
    else {
      keyEntered += keypressed;
    }

    //updates lcd output
    //    lcd.clear(); //resets the lcd display
    //    lcd.setCursor(0, 0); //Start at character 4 on line 0
    //    lcd.print("OTP: ");
    //    lcd.print(code); //shows the same otp over and over until
    lcd.setCursor(0, 1); //to print the key that is currently typed in
    lcd.print("                ");
    lcd.setCursor(0, 1); //to print the key that is currently typed in
    lcd.print(keyEntered);
  }

  //long GMT = rtc.getTimestamp(); //this is for the softRTC internal to arduino

  //Serial.print("UNIX Time: ");
  //Serial.print(RTC.get());
  long GMT = RTC.get(); //this is to get the time from the hardware RTC
  char* newCode = totp.getCode(GMT);
  if (strcmp(code, newCode) != 0) {
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

    //lcd outputs
    lcd.clear();
    lcd.setCursor(0, 0); //Start at character 4 on line 0
    lcd.print("OTP: ");
    lcd.print(code);
    lcd.setCursor(0,1);
    lcd.print(keyEntered);

  }
}

void print2digits(int number) {
  if (number >= 0 && number < 10)
    Serial.write('0');
  Serial.print(number);
}

void codeChecker(String inputKey) { //checks the code when enter key is pressed
  Serial.println("Entered Code Checker");
  if (String(inputKey) == String(code)) { //if code is correct, send correct signal out
    Serial.println("Code is Correct!");
    analogWrite(A1, 255);
    delay(2000);
    analogWrite(A1, 0);
  }
  else {
    Serial.println("Code is Wrong!");
    analogWrite(A0, 255);
    delay(2000);
    analogWrite(A0, 0);
  }
}

