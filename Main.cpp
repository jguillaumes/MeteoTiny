#include <Arduino.h>
#include <TinyBMP280.h>
#include <SoftwareSerial.h>
#include <dht.h>
#include <Time.h>

//+
// Test sketch for the TinyBMP280 library.
// For use with an Attiny85:
// - Connect BMP280 SCL to pin 7
// - Connect BMP280 SDA to pin 8
// - Connect BMP280 SDO to ground
// - Connect a serial terminal using:
//   - TX to Pin 2 (corresponds to Arduino pin 3)
//   - RX to pin 3 (corresponds to Arduino pin 4)
// - Setup your terminal or serial monitor to 2400 bauds
// - Enjoy!

#define DHT_PIN 1
#define DELAY_TIME 5000
#define FW_VERSION "T1.00.00"
#define HW_VERSION "T1.00.00"

SoftwareSerial mySerial(3, 4);
tbmp280::TinyBMP280 bmp;
char isHigro = 'H';

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


void setup() {
	mySerial.begin(9600);
	bmp.begin();

	delay(2500);
	mySerial.println("\nBEGIN:");
	mySerial.print("INFO :F"); mySerial.print(FW_VERSION);
	mySerial.print(":H"); mySerial.println(HW_VERSION);
}

void loop() {
	char buff[80];
	time_t now;

	now = millis();
	uint32_t t = bmp.readIntTemperature();
	uint32_t p = bmp.readIntPressure();
	int32_t h = readHumidity();

	sprintf(buff, "DATA :M%09lu:T%04lu:P%06lu:H%05ld:F%s:W%s:D-T%cP-" ,
				now, t, p, h,  FW_VERSION, HW_VERSION, isHigro);
	mySerial.println(buff);
	delay(DELAY_TIME);
}
