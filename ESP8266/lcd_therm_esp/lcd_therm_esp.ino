//#include "nodeMCU_GPIO.h"
/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
    ***USING NODEMCU D* PIN MAPPING
 * LCD RS pin to digital pin D1
 * LCD Enable pin to digital pin D2
 * LCD D4 pin to digital pin D4
 * LCD D5 pin to digital pin D5
 * LCD D6 pin to digital pin D6
 * LCD D7 pin to digital pin D7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include "pins_arduino.h"
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(D1, D2, D4, D5, D6, D7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
    //Serial.begin(9600);
    //Serial.println(A0);
    //Serial.println(SDA);
    //Serial.println(SCL);
  // Print a message to the LCD.
  //lcd.print("Current temp:");
}
int tempPin = A0;
void loop() {
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 0);

    int tempReading = analogRead(tempPin);
    // This is OK
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
    float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
    float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit

    lcd.print(tempC);
    lcd.print(" C ");
    lcd.setCursor(0, 1);
    lcd.print(tempF);
    lcd.print(" F ");
    delay(500);
}

