#include <Servo.h>

#define CLK_PIN	3
#define DT_PIN	2
#define SW_PIN	4
#define SERVO_PIN	9

int pos = 180;
Servo servo;

int previousClockState;
int currentClockState;

void setup()
{
	Serial.begin (9600);
	pinMode (CLK_PIN, INPUT);
	pinMode (DT_PIN, INPUT);
	pinMode (SW_PIN, INPUT_PULLUP);
	servo.attach (SERVO_PIN);
	previousClockState = digitalRead (CLK_PIN);
}

void loop()
{
	servo.write (pos);
	currentClockState = digitalRead (CLK_PIN);
	if (currentClockState != previousClockState) {
		if (digitalRead (DT_PIN) != currentClockState) {	//indicates clockwise rotation
			pos = (pos < 180) ? pos + 1 : pos;
		} else {
			pos = (pos > 0) ? pos - 1 : pos;
		}
	}
	Serial.println (pos);
	previousClockState = currentClockState;
}
