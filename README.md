# MeteoTiny - ATTiny version of the MeteoArduino sensor firmware

This project contains the firmware to support a minimal set of meteorological sensors driven by an Attiny class MCU. It is a "cheap" alternative to MeteoArduino, which uses an ATMega328P MCU.

## Sensors

The sensors supported by this firmware are:

- BMP280 temperature and pressure sensor
- DHT22 humidity and temperature sensor

The pressure and temperature are taken from the BMP280 and are read and reported as integers to avoid linking in the floating point support. There is no realtime clock support: the firmware sends to the client a millisecond count (from the device startup). The client software is responsible of taking care of converting that millisecond delta to real time (date and time).

There is basically no retry logic nor failover mechanism for sensors. The BMP280 **must** work all the time. The DHT22 **can** fail, and in that case the humidity value is reported as -999, and the hygrometer flag in the :D field is set to '-'.

## Communications

This firmare just establishes a software serial interface using the pins 2 and 3 of the ATTiny85 (corresponding to 3 and 4 in Arduino numbering) and sends the DATA: messages *in the blind*. The idea is to connect a pre-configured HC-06 BT module there. The module will start to send messages once paired from the client. The first line sent will be probably incomplete, so the client has to take precaution to discard truncated or incomplete messages. 

There is no provision to answer commands, and actually there is no logic to handle incoming data.



