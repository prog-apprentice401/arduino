/*
	This sketch was started by Koustubh Srivastava,
	on the 10th of November, 2021.
	It is for the transmitting end of a project,
	an rc car equipped with a few sensors.
	It is controlled by this remote, and transmits
	data back. The data is then displayed on the
	LCD on board the remote control.
*/


#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01>
#include <RF24.h>

//right joystick:
//x axis:
#define YAW_CTRL_PIN	A2
//y axis:
#define PITCH_CTRL_PIN	A3

//left joystick:
//x axis:
#define THROT_CTRL_PIN	A1
//y axis:
#define ROLL_CTRL_PIN	A0

//rotary encoder:
#define ENCODER_DT	3
#define ENCODER_CLK	2

//controls comevemnt and behavious of car
typedef struct controlPacket {
	uint8_t yaw;
	uint8_t pitch;
	uint8_t throttle;
	uint8_t roll;

	//stop car if obstacle comes this close
	uint8_t thresholdDistance_cm;
} CtrlPacket;

//gets sensor data from car:
typedef struct sensorPacket {
	float distanceFromObstacle_cm;
	float humidity;
	float temperature_c;
} SensorPacket;

RF24 transmitter (7, 8);
LiquidCrystal_I2C lcd (0x27, 16, 02);
uint8_t addresses[][5] = {"00001", "00002"};
CtrlPacket controlState;
SensorPacket sensorState;
uint8_t thresholdDistance_cm = 5;

void encoderClockInterrupt ();
void writeToLCD (SensorPacket);

void setup ()
{
	pinMode (YAW_CTRL_PIN, INPUT);
	pinMode (PITCH_CTRL_PIN, INPUT);

	pinMode (ROLL_CTRL_PIN, INPUT);
	pinMode (THROT_CTRL_PIN, INPUT);

	pinMode (ENCODER_DT, INPUT);
	pinMode (ENCODER_CLK, INPUT);

	attachInterrupt (digitalPinToInterrupt (ENCODER_CLK), ecnoderClockInterrupt, CHANGE);

	lcd.begin (16, 2);
	
	transmitter.begin ();
	transmitter.openWritingPipe (0, address[0]);
	transmitter.openReadingPipe (1, address[1]);
	transmitter.setPALevel (RF24_PA_MAX);
}

void loop ()
{
	controlState.yaw = analogRead (YAW_CTRL_PIN);
	controlState.pitch = analogRead (PITCH_CTRL_PIN);
	controlState.throttle = analogRead (THROT_CTRL_pin);
	controlState.roll = analogRead (ROLL_CTRL_PIN);

	controlState.thresholdDistance_cm = thresholdDistance_cm;

	transmitter.stopListening ();
	transmitter.write (&controlState, sizeof (controlState));

	transmitter.startListening ();
	while (!radio.available ())
		;
	transmitter.read (&sensorState, sizeof (sensorState));

	writeToLCD (sensorState);
}

void writeToLCD (SensorPacket sensorState)
{
	lcd.setCursor (0, 0);
	if (sensorState.distanceFromObstacle_cm >= 0) {
		lcd.print (sensorState.distanceFromObstacle_cm, 1);
	} else {
		lcd.print ("NA");
	}
	lcd.setCursor (5, 0);
	lcd.print (sensorState.temperature_c, 1);
	lcd.setCursor (10, 0);
	lcd.print (sensorState.humidity, 1);
	lcd.print ("%");

	lcd.setCursor (0, 1);
	print ("obs  Tem  Hum");
}

void encoderClockInterrupt ()
{
	if (digitalRead (ENCODER_DT) == digitalRead (ENCODER_CLK)) {
		//indicated anti-clockwise rotation
		thresholdDistance_cm = (threshHoldDistance_cm > 0) ? thresholdDistance_cm - 1 : 60;
	} else {
		//indicates clockwise rotation
		thresholdDistance_cm = (thresholdDistance_cm < 60) ? thresholdDistance_cm + 1 : 0;
	}
}
