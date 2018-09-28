#include <Arduino.h>

#include <TinyBMP280.h>

#include <SoftwareSerial.h>
#include <dht.h>

//+
// Meteorological sensors - ATTiny85 version
// Uses a BMP280 or BMP180 to measure temperature and pressure and a DHT22 for humidity
// Connections:
// - BMP280:
//   - Connect the SCL and SDA to the pins 7 and 5 of the Attiny (Arduino equivalent
//     pins are 2 and 0)
//   - Connect the SDO pin to ground (I use a 10K resistor)
// - BMP180:
//   - Connect the SCL and SDA to the pins 7 and 5 of the Attiny (Arduino equivalent
//     pins are 2 and 0)
// - DHT22:
//   - Connect the signal leg to the pin 7 of the Attiny (Arduino equivalent is 1)
// - HC06:
//   - Connect TX to pin 2 (Arduino equivalent is pin 3)
//   - Connect RX to pin 3 (Arduino equivalent is pin 4)
// Setup VCC (3.3V) and GND, and you are set to go
//
// The sketch will begin to transmit data as soon as the BT device is paired. The
// data is sent as integers (divide temperature and pressure by 100 to get
// centigrades and hectopascals, the humidity value is the relative %).
//
// This sketch does not do any timekeeping. Instead of the ":C" field with a timestamp it
// sends a ":M" field with the milliseconds since the MCU was booted. The client has to
// convert these readings to real times.
//
// It requires the following libraries:
// - DHTlib (to drive the DHT22)
// - SoftwareSerial (to talk to the HC-06)
// - TinyBMP280 (to talk to the BMP280) (https://github.com/jguillaumes/TinyBMP280)
// - TinyWireM (required by TinyBMP280)
//-

#define DHT_PIN 1
#define DELAY_TIME 5000
#define FW_VERSION "T1.00.00"
#define HW_VERSION "T1.00.00"

SoftwareSerial mySerial(3, 4);
tbmp::TinyBMP180 bmp;			// Select TinyBMP280 or TinyBMP180
char isHigro = 'H';

//+
// Read hygrometer (DHT22) measurement
//-
int32_t readHumidity() {
	dht hig;

	int32_t h;
	int chk;

	chk = hig.read22(DHT_PIN);
	if (chk == DHTLIB_OK) {
		h = hig.humidity;
		isHigro = 'H';
	} else {
		h = -999;
		isHigro = '-';
	}
	return h;
}

//+
// Sketch preparation
//-
void setup() {
	mySerial.begin(9600);
	bmp.begin();

	delay(2500);
	mySerial.println("\nBEGIN:");
	mySerial.print("INFO :F"); mySerial.print(FW_VERSION);
	mySerial.print(":H"); mySerial.println(HW_VERSION);
}

//+
// Main loop
// This sketch is quite dumb. It just loops reading the sensor data
// and sends them in a formatted line.
// There is no real time clock, so we send just a milliseconds delta
// since the MCU was booted.
//-
void loop() {
	char buff[80];

	long now;

	now = millis();

	int32_t t = bmp.readIntTemperature();
	uint32_t p = bmp.readIntPressure();
	int32_t h = readHumidity();

	sprintf(buff, "DATA :M%09lu:T%05ld:P%06lu:H%05ld:F%s:W%s:D-T%cP-" ,
				now, t, p, h,  FW_VERSION, HW_VERSION, isHigro);
	mySerial.println(buff);
	delay(DELAY_TIME);
}
