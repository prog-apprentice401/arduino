/*
	This project can use PIR sensors with a delay of ~ 2 seconds (more for houses with slow walkers,
	less for those with fast), or proximity sensors.
	NOTE- for proximity sensors, a monostable circuit with a stable state of low, and a delay of ~ 2 secs with needed.
	This will make sure the microcontroller can act the same with both of them.

	Authored by prog-apprentice401 (real name - Koustubh Srivastava)
	on 19th April, 2022
*/
#define MIC_PIN A0
#define SENSOR_ONE_PIN 2
#define SENSOR_TWO_PIN 3
#define RELAY_PIN 6

#define MIC_THRESHOLD 400
#define CLAP_TIMEOUT 500	//maximum time between two claps
#define CLAP_END_TIMEOUT 1000	//minimum 'soundless' time after clap to ensure clap was for appliance

volatile int persons = 0; //stores current number of persons in room
volatile bool relaySwitchState = LOW;
int micReading = 0;
unsigned long int lastClap = 0;
bool isClapValid = false;

//executed when sensor 1 turns on
void sensorOneOn (void)
{
	//sensor two pin is already HIGH when sensor 1 goes high,
	//i.e., sensor 2 turned on before sensor 1
	//so, a person is going out
	if (digitalRead (SENSOR_TWO_PIN) == HIGH) {	
		persons--;
		if (persons <= 0) {
			relaySwitchState = LOW;
		}
	} else {
		persons++;
		if (persons == 1) {
			relaySwitchState = HIGH;
		}
	}
}

void setup ()
{
	pinMode (MIC_PIN, INPUT);
	pinMode (SENSOR_1, INPUT);
	pinMode (SENSOR_2, INPUT);
	pinMode (RELAY_PIN, OUTPUT);

	attachInterrupt (digitalPinToInterrupt (SENSOR_ONE_PIN), sensorOneOn, RISING);
}

void loop ()
{
	if (isClapValid) {	//switch lights on valid claps
		relaySwitchState = !relaySwitchState;
	}
	digitalWrite (RELAY_PIN, relaySwitchState);
	micReading = analogRead (MIC_PIN);
	isClapValid = false;

	if (micReading < MIC_THRESHOLD) {	//doesn't let program continue if no clap was sounded
		continue;
	}
	lastClap = millis ();

	while (millis () - lastClap >= CLAP_TIMEOUT) {	//listen for second clap
		micReading = analogRead (MIC_PIN);
		if (micReading >= MIC_THRESHOLD) {	//valid second clap
			isValidClap = true;
			lastClap = millis ();
			break;
		}
	}

	if (!isClapVaid) {	//don't move forward for invalid claps
		continue;
	}

	while (millis () - lastClap <= CLAP_END_TIMEOUT) {	//make sure there is no third clap
		micReading = analogRead (MIC_PIN);
		if (micReading >= MIC_THRESHOLD) {	//third clap in s
			isClapValid = false;
			break;
		}
	}
}
