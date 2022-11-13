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

//left joystick:
//x axis:
#define YAW_CTRL_PIN	A2
//y axis:
#define THRUST_CTRL_PIN	A3

//left joystick:
//x axis:
#define ROLL_CTRL_PIN	A1
//y axis:
#define THROT_CTRL_PIN	A0

//rotary encoder:
#define ENCODER_DT	3
#define ENCODER_CLK	2

//Packet of data to send
typedef struct controlPacket {
	uint8_t yaw;
	uint8_t pitch;
	uint8_t throttle;
	uint8_t roll;

	//whether encoder is moved, and in which direction
	//possible values -- -1:ccw ; 0:no movement ; 1:cw
	int8_t encoder_diff;
} CtrlPacket;

//gets data from reciever, to be used as wished:
typedef struct sensorPacket {
	float receptionChannel1;
	float receptionChannel2;
	float receptionChannel3;
} SensorPacket;

//-1 for ccw, 1 for cw, 0 for no motion
volatile int8_t encoderDiff = 0;

RF24 transmitter (7, 8);
LiquidCrystal_I2C lcd (0x27, 16, 02);
uint8_t addresses[][5] = {"00001", "00002"};
CtrlPacket controlState;
SensorPacket sensorState;

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
	controlState.yaw = map (analogRead (YAW_CTRL_PIN), 0, 1023, 0, 255);
	controlState.pitch = map (analogRead (PITCH_CTRL_PIN), 0, 1023, 0, 255);
	controlState.throttle = map (analogRead (THROT_CTRL_pin), 0, 1023, 0, 255);
	controlState.roll = map (analogRead (ROLL_CTRL_PIN), 0, 1023, 0, 255);

	controlState.encoderDiff = encoderDiff;

	transmitter.stopListening ();
	transmitter.write (&controlState, sizeof (controlState));
	encoderDiff = 0;

	transmitter.startListening ();

	//if data can be read, read it, else, continue giving control instructions
	if (radio.available ()) {
		transmitter.read (&sensorState, sizeof (sensorState));
		writeToLCD (sensorState);
	}
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
		encoderDiff = -1
	} else {
		//indicates clockwise rotation
		encoderDiff = 1;
	}
}
