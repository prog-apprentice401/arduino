#define LED_PIN		3
#define INPUT_PIN	A0
#define THRESHOLD	50

int16_t ambientReading;
uint8_t recievedData = 0;
uint8_t readBit;

void setup ()
{
	Serial.begin (9600);
	pinMode (LED_PIN, OUTPUT);
	pinMode (INPUT_PIN, INPUT);
	digitalWrite (LED_PIN, LOW);

	//first reading of solar panel is incorrect
	analogRead (INPUT_PIN);	
	delay (50);
}

void loop ()
{
	ambientReading = analogRead (INPUT_PIN);
	recievedData = 0;
	for (int i = 0; i < 8; i++) {
		while ((analogRead (INPUT_PIN) > ambientReading + THRESHOLD ? 1 : 0) == readBit)
			;
		delay (10);
		readBit	 = analogRead (INPUT_PIN) > ambientReading + THRESHOLD ? 1 : 0;
		recievedData |= readBit << i;
		Serial.println (readBit, BIN);
	}
	Serial.println (recievedData, BIN);
	analogWrite (LED_PIN, recievedData);
	delay (10);
}
