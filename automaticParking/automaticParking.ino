#include <Servo.h>

#define TRIG_PIN	2
#define ECHO_PIN	3
#define SERVO_PIN	9
#define DELAY_BEFORE_CLOSE	3000

int distance;
bool isCarAtGate = false;

Servo gateServo;

int getDuration (int, int);
void gateClose (Servo gateServo);
void gateOpen (Servo gateServo);

void setup ()
{
	Serial.begin (9600);
	pinMode (TRIG_PIN, OUTPUT);
	pinMode (ECHO_PIN, INPUT);
	gateServo.attach (SERVO_PIN);

	gateClose (gateServo);
}

void loop ()
{
	distance = getDistance (TRIG_PIN, ECHO_PIN);

	if (!isCarAtGate && distance <= 15) {  //car is sensed right now
		gateOpen (gateServo);
		isCarAtGate = true;
	} else if (isCarAtGate && distance >= 15) {  //car just passed
		Serial.println ("Car Passed, timing");
		delay (3000);
		gateClose (gateServo);
		isCarAtGate = false;
	}
}

int getDistance (int trigPin, int echoPin)
{
	unsigned long int duration;
	int cm;
	digitalWrite (trigPin, LOW);
	delay (2);
	digitalWrite (trigPin, HIGH);
	delayMicroseconds (10);
	digitalWrite (trigPin, LOW);
	duration = pulseIn (echoPin, HIGH);
	cm = duration/58.82l;

	return cm;
}

void gateOpen (Servo gateServo)
{
	gateServo.write (0);
	delay (500);
}

void gateClose (Servo gateServo)
{
	gateServo.write (90);
	delay (500);
}
