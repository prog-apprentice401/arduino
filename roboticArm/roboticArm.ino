/*
	this code allows the arduino to ccontrol the 
	input acording to the position of potentiomters,
	fit inside of a small replica
	and thus the arm replicates the position of the replica

	after making the true arm, we have to make a 
	small replica using potentiometers instead of servos
	instead of making the replica, we can simply connect the
	POTS and that way, we do not need to make sure that the rotation
	of the POTs and the servos match

	this code allows the arm to have five joints
	for the rotation of the whole structure
	for the rotation of the claw
	for opening and closing the claw
	one 'elbow' joint, allowing it to move up and down
	and a lower 'elbow' to control the range of the arm
*/



#include<Servo.h>

#define base_POT 0
#define elbow_low 1
#define elbow_high 2
#define claw_control 3
#define claw_rotate 4

Servo baseServo;
Servo elbowLow;
Servo elbowHigh;
Servo Claw;
Servo ClawRotate;

void setup () {
	baseServo.attach (3);
	elbowLow.attach (5);
	elbowHigh.attach (6);
	Claw.attach (9);
	ClawRotate.attach (10);
}

void loop () {
	byte basePOS = map (analogRead (base_POT), 0, 1023, 0, 180);
	byte elbowLowPOS = map (analogRead (elbow_low), 0, 1023, 0, 180);
	byte elbowHighPOS = map (analogRead (elbow_high), 0, 1023, 0, 180);
	byte clawPOS = map (analogRead (claw_control), 0, 1023, 0, 180);
	byte rotatePOS = map (analogRead (claw_rotate), 0, 1023, 0, 255);
 
	baseServo.write (basePOS);
	elbowLow.write (elbowLowPOS);
	elbowHigh.write (elbowHighPOS);
	Claw.write (clawPOS);
	ClawRotate.write (rotatePOS);
}
