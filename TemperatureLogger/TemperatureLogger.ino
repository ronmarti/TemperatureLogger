#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal.h>

const int PIN_ONBOARD_DIODE = 13,
PIN_TMP_SENS_SCL = A5,
PIN_TMP_SENS_SDA = A4,
PIN_CHIP_SELECT_SDCARD = 22;

const int COEF_CORRECTION = 32;

const String FILE_LOG_NAME = "logBaro2.csv";

bool state = false;

float temp = 0,
alt = 0,
pres = 0;

Adafruit_BMP085 bmp180;

File dataFile;

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

	if (!SD.begin(PIN_CHIP_SELECT_SDCARD)) {
		Serial.println("Card failed, or not present");
		// don't do anything more:
		return;
	}
	appendDataToSDCard("Temperature [C], Pressure [hPa], Altitude [m]");
	Serial.println("SD card initialized.");

}

void loop()
{

	delay(500);
	digitalWrite(PIN_ONBOARD_DIODE, state);
	state = !state;

	readBMP180Data();
	printSerialBMP180();

	appendDataToSDCard(composeBMPDataForLog());
}

String composeBMPDataForLog(){
	String dataString = "";

	dataString += temp;
	dataString += ", ";
	dataString += pres;
	dataString += ", ";
	dataString += alt;
	return dataString;
}

void appendDataToSDCard(String dataString){
	dataFile = SD.open(FILE_LOG_NAME, FILE_WRITE);
	if (dataFile) {
		dataFile.println(dataString);
		dataFile.close();
	}
	// if the file isn't open, pop up an error:
	else {
		Serial.println("error opening datalog.txt");
	}
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
