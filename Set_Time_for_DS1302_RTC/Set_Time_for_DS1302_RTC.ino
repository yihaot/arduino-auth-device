 
#include <DS1302.h>
 
// Init the DS1302
DS1302 rtc(5, 6, 7);    // Change the pins here if you want
 
void setup()
{
// Set the clock to run-mode, and disable the write protection
rtc.halt(false);
rtc.writeProtect(false);
 
// Setup Serial connection
Serial.begin(9600);
 
// The following lines can be commented out to use the values already stored in the DS1302
// Once you flash the arduino with the correct time.
 
rtc.setDOW(SATURDAY); // Set Day-of-Week to FRIDAY
rtc.setTime(7, 44, 40); // Set the time to 12:00:00 (24hr format)
rtc.setDate(17, 9, 2016); // Set the date to August 6th, 2010
}
 
void loop()
{
// Send Day-of-Week
Serial.print(rtc.getDOWStr());
Serial.print(" ");
 
// Send date
Serial.print(rtc.getDateStr());
Serial.print(" -- ");
 
// Send time
Serial.println(rtc.getTimeStr());
 
// Wait one second before repeating :)
delay (1000);
}
