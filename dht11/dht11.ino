#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define TYPE 11
#define input 8

DHT dht (input, TYPE);

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup () {
	pinMode (8, INPUT);
	lcd.begin (16, 2);
	lcd.clear ();
	dht.begin ();
}

void loop () {
	uint8_t temperature, humidity;

	humidity = dht.readHumidity ();
	temperature = dht.readTemperature ();
	
	lcd.clear ();
	lcd.print ("Humidity: ");
	lcd.print (humidity);
	lcd.print ("%");
	lcd.setCursor (0, 1);
	lcd.print ("Temperature: ");
	lcd.print (temperature);
	lcd.print ("C");

	delay (500);
}
