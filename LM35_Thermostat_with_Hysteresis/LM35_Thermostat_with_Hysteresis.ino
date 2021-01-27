/*
Name:       LM35_Thermostat_with_Hysteresis.ino
Function:	Simple LM35-based thermostat with settable setpoint and hysteresis
			This sketch uses the analog value acquired on A0 to configure the setpoint

Created:		2018-06-13
Last Modified:	2020-11-03

Copyright (c) 2018, Francesco Adamo - Polytechnic University of Bari - Italy
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
#define K_LM35		Q/0.01	// transducer constant [degC/LSB]

#define HEATER_OUTPUT		12	// Digital output pin to control the heater
#define FAN_OUTPUT			11	// Digital output pin to control the fan
#define LM35_ANALOG_IN		A0	// Analog pin to read the LM35 output
#define SETPOINT_ANALOG_IN	A1	// Analog pin to read the setpoint
#define DUMMY_ANALOG_IN		A2

#define	MIN_TEMP		(double) 20.0	// Minimum settable temperature [degC]
#define MAX_TEMP		(double) 40.0	// Maximum settable temperature [degC]
#define HYST			(double) 2	// Regulation hysteresis [degC]

// Macro definition
// NOTE: SWITCH_XXX_ON/OFF puts LOW/HIGH on the digital output to take into account the inverted
//		 characteristic of the commonly available relay modules
#define SWITCH_HEATER_OFF()	(digitalWrite(HEATER_OUTPUT, HIGH))
#define SWITCH_HEATER_ON()	(digitalWrite(HEATER_OUTPUT, LOW))

#define SWITCH_FAN_OFF()	(digitalWrite(FAN_OUTPUT, HIGH))
#define SWITCH_FAN_ON()		(digitalWrite(FAN_OUTPUT, LOW))

double sp, t, dummy, LT, HT; // setpoint, actual temperature, low threshold, high threshold


void setup()
{
	//analogReference(INTERNAL); // Switch the ADC reference input to the 1.1 V internal reference. Uncomment if VREF = 1.1
	//sp = MIN_TEMP + analogRead(SETPOINT_ANALOG_IN)*(MAX_TEMP - MIN_TEMP) / 1024; // read analog input 0 and scales value to [MIN_TEMP, MAX_TEMP] interval
	pinMode(HEATER_OUTPUT, OUTPUT);
	pinMode(FAN_OUTPUT, OUTPUT);
	SWITCH_HEATER_OFF();
	SWITCH_FAN_OFF(); 
	
	Serial.begin(115200);
	Serial.println("Arduino + LM35 Thermostat w/ hysteresis and settable threshold");
}

void loop()
{
	delay(100);
	//dummy = analogRead(DUMMY_ANALOG_IN); // Alternative way to permit the settling of signals between MUX switching
	sp = MIN_TEMP + analogRead(SETPOINT_ANALOG_IN)*(MAX_TEMP - MIN_TEMP) / 1024; // Scale the analog input value to [MIN_TEMP, MAX_TEMP] interval
	//dummy = analogRead(DUMMY_ANALOG_IN);
	t = K_LM35 * analogRead(LM35_ANALOG_IN); // dummy reading of LM35 input to permit the input settling
	t = K_LM35*analogRead(LM35_ANALOG_IN);

	LT = sp - HYST / 2;
	HT = sp + HYST / 2;

	// Thermostat w/ Hysteresis implementation
	if (t < LT) {

		// Temperature less than the lower threshold => switch off the fan!
		SWITCH_HEATER_ON();
		SWITCH_FAN_OFF();
	}
	else if (t > HT) {

		// Temperature greater than the higher threshold => switch on the fan!
		SWITCH_HEATER_OFF();
		SWITCH_FAN_ON();
	}
	else {
		// Temperature is in the hysteresis window => don't do anything!
	}

	// Sends a string formatted as <LT>, <HT>, <t>, <relay_status>CRLF over the serial port
	Serial.print(LT, 1);
	Serial.print(", ");
	Serial.print(HT, 1);
	Serial.print(", ");
	Serial.print(t, 1);
	Serial.print(", ");
	Serial.print(digitalRead(HEATER_OUTPUT) ? "H_OFF":"H_ON ");
	Serial.print(", ");
	Serial.println(digitalRead(FAN_OUTPUT) ? "F_OFF" : "F_ON ");
}
