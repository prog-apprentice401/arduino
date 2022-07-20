#include <Servo.h>

#define TRIG_PIN	2
#define ECHO_PIN	3
#define SERVO_PIN	9

unsigned long millisAtGatePassing;
int cm;
bool isGateOpen = false;
bool isCarAtGate = false;

servo gateServo;


void setup ()
{
	pinMode (TRIG_PIN, OUTPUT);
	pinMode (ECHOPIN, INPUT);
	gateServo.attach (SERVO_PIN);

	gateClose (gateServo);
}

void loop ()
{
	cm = getDistance (TRIG_PIN, ECHO_PIN);
	if (!isGateOpen && cm <= 15) {
		gateOpen (gateServo);
		isGateOpen = true;
		isCarAtGate = true;
	} else if (isGateOpen && cm > 15) {
		//the car has gone just now, hence, we will update car status
		if (isCarAtGate) {
			millisAtGatePassing = millis ();
			isCarAtGate = false;
		} else if (millis () - millisAtGatePassing >= DELAY_BEFORE_CLOSE) {
			gateClose (gateServo);
			isGateOpen = false;
		}
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

	duration = pulsein (echoPin, HIGH);
	cm = duration/58.82l

	return cm;
}

void gateOpen (servo gateServo)
{
	gateServo.write (0);
	delay (500);
}

void gateClose (servo gateServo)
{
	gateServo.write (90);
	delay (500);
}
