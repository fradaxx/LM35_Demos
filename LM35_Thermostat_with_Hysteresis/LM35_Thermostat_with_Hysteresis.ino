/*
Name:       LM35_Thermostat_with_Hysteresis.ino
Function:	Simple LM35-based thermostat with settable setpoint and hysteresis
			This sketch uses the analog value acquired on A0 to configure the setpoint

Created:		2018-06-13
Last Modified:	2022-12-05

Copyright (c) 2018-2022, Francesco Adamo - Polytechnic University of Bari - Italy
e-mail: francesco.adamo@poliba.it

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

// defines
#define ADC_FSR		5.0 // ADC Full Scale Range [V]
#define Q			(ADC_FSR/1024.0)	// ADC resolution [V/LSB]
#define K_LM35		Q/0.01	// transducer constant [°C/LSB]

#define HEATER_OUTPUT		12	// Digital output pin to control the heater
#define FAN_OUTPUT			11	// Digital output pin to control the fan
#define LM35_ANALOG_IN		A0	// Analog pin to read the LM35 output
#define SETPOINT_ANALOG_IN	A1	// Analog pin to read the setpoint
#define DUMMY_ANALOG_IN		A2

#define	TMIN		(double) 15.0	// Minimum settable temperature [°C]
#define TMAX		(double) 40.0	// Maximum settable temperature [°C]
#define HYST		(double) 2	// Regulation hysteresis [°C]
#define m (double) (TMAX - TMIN)/1024 // scaling factor (from sp input voltage to temperature) [°C/LSB]

// Definition of ON/OFF symbols to simplify relay module drive
// There are relays modules that are "active low" (i.e. they are ON on the LOW state of the driving digital input 
// and other that are "active high", i.e. they are ON on the HIGH status of the driving digital input
#define ON      LOW
#define OFF     HIGH

// Macro definition
#define HEATER(status) (digitalWrite(HEATER_OUTPUT, status))
#define FAN(status)	(digitalWrite(FAN_OUTPUT, status))

double sp, t, dummy, lt, ht; // setpoint, actual temperature, low threshold, high threshold


void setup()
{
	//analogReference(INTERNAL); // Switch the ADC reference input to the 1.1 V internal reference. Uncomment if VREF = 1.1
	pinMode(HEATER_OUTPUT, OUTPUT);
	pinMode(FAN_OUTPUT, OUTPUT);
	HEATER(OFF);
	FAN(OFF); 
	Serial.begin(115200);
	Serial.println("LM35 Thermostat w/ histeresys");
	Serial.println("setpoint temperature heater fan");
}

void loop()
{
	sp = TMIN + m*analogRead(SETPOINT_ANALOG_IN); // actual setpoint in degrees Celsius
	t = K_LM35 * analogRead(LM35_ANALOG_IN); // actual temperature in degrees Celsius
	t = K_LM35 * analogRead(LM35_ANALOG_IN); // actual temperature in degrees Celsius

	lt = sp - HYST / 2;
	ht = sp + HYST / 2;

	// Thermostat w/ Hysteresis implementation
	if (t < lt) {

		// Temperature less than the lower threshold => switch heater on an fan off!
		HEATER(ON);
		FAN(OFF);
	}
	else if (t > ht) {

		// Temperature greater than the higher threshold => switch heater off and fan on!
		HEATER(OFF);
		FAN(ON);
	}
	else {
		// Temperature is in the hysteresis window => don't do anything!
	}

	// Sends a string formatted as <lt>, <ht>, <t>, <heater_status>, <fan_status>CRLF over the serial port
	Serial.print("/*"); // needed as start of frame by Serial Studio
	Serial.print(lt, 1);
	Serial.print(", ");
	Serial.print(ht, 1);
	Serial.print(", ");
	Serial.print(t, 1);
	Serial.print(", ");
	Serial.print(digitalRead(HEATER_OUTPUT) ? "0":"1");
	Serial.print(", ");
	Serial.print(digitalRead(FAN_OUTPUT) ? "0":"1");
	Serial.println("*/"); // needed as end of frame by Serial Studio

	delay(100);
}
