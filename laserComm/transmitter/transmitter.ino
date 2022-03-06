#define POT_PIN A0
#define OUTPUT_PIN 2
#define SIGNAL_PIN 3

uint8_t potValue;
void sendThroughLaser (const byte, uint8_t);

void setup ()
{
	Serial.begin (9600);
	pinMode (POT_PIN, INPUT);
	pinMode (OUTPUT_PIN, OUTPUT);
	pinMode (SIGNAL_PIN, INPUT_PULLUP);
	digitalWrite (OUTPUT_PIN, LOW);
}

void loop ()
{
	while (digitalRead (SIGNAL_PIN));

	potValue = map (analogRead (POT_PIN), 0, 1023, 0, 255);
	sendThroughLaser (OUTPUT_PIN, potValue);
	delay (20);
}

void sendThroughLaser (const byte outputPin, uint8_t dataToSend)
{
	bool currentOutputState = LOW;
	uint8_t temp = dataToSend;
	for (int i = 0; i < 8; i++){
		digitalWrite (outputPin, !currentOutputState);
		delay (5); //signalling new bit
		currentOutputState = temp & B00000001;	//access LSB of data
		temp >>= 1;
		digitalWrite (outputPin, currentOutputState);	
		delay (10);
		Serial.println (currentOutputState);
	}
	digitalWrite (outputPin, LOW);
	Serial.println (dataToSend, BIN);
}
