/*
This is a sketch that demonstrates the use of I2C LCD screens, and the HC-SR04 distance sensor.

*Created by Koustubh Srivastava
*on 10th January 2020
*/

#include <LiquidCrystal_I2C.h>

#define TRIG 2
#define ECHO 3

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned short duration;
uint8_t distance;

void setup ()
{
	Serial.begin(9600);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
	lcd.begin(16, 2);
}

void loop ()
{	
	digitalWrite(TRIG, LOW);
	delayMicroseconds(2);
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);

	duration = pulseIn(ECHO, HIGH);

	distance = (int)duration * 0.034/2;

	if (distance > 40) {
		distance = 0;
	}
	if (distance < 3) {
		distance = 0;
	}
	lcd.setCursor(0, 0);
	lcd.clear();
	lcd.print("Distance in CMs:");
	lcd.setCursor(0, 1);
	lcd.print(distance);

	Serial.println (distance);
	
	duration = 0;
	distance = 0;
	
	delay(100);
}
