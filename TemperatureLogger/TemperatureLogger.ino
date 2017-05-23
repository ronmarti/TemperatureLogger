#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal.h>

const int PIN_ONBOARD_DIODE = 13,
			PIN_TMP_SENS_SCL = A5,
			PIN_TMP_SENS_SDA = A4;

const int COEF_CORRECTION = 32;

bool state = false;

float temp = 0,
alt = 0,
pres = 0;

Adafruit_BMP085 bmp180;

void setup()
{
	Serial.begin(115200);

	pinMode(PIN_ONBOARD_DIODE, OUTPUT);
	pinMode(PIN_TMP_SENS_SCL, INPUT);
	pinMode(PIN_TMP_SENS_SDA, INPUT);

	delay(500);

	if (!bmp180.begin()) {
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	}

}

void loop()
{

	delay(500);
	digitalWrite(PIN_ONBOARD_DIODE, state);
	state = !state;

	readBMP180Data();
	printSerialBMP180();
}


void readBMP180Data(){
	temp = bmp180.readTemperature();
	pres = (bmp180.readPressure() + COEF_CORRECTION * 100) / 100.00;
	alt = bmp180.readAltitude(bmp180.readPressure() + COEF_CORRECTION * 100);
}

void printSerialBMP180(){
	printSerialBMP180(temp, pres, alt);
}

void printSerialBMP180(float temperature, float pressure, float altitude){
	Serial.print("Teplota: ");
	Serial.print(temperature);
	Serial.print(" °C | ");

	Serial.print("Tlak: ");
	Serial.print(pressure);
	Serial.print(" hPa | ");

	Serial.print("Nadmorska vyska prepoctena: ");
	Serial.print(altitude);
	Serial.println(" m n. m.");
	Serial.println("------------------------------");
}
