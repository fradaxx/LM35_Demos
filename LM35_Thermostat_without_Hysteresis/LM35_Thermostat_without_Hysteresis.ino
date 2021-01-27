/*
Name:       LM35_Thermostat_whitout_Hysteresis.ino
Function:	Simple LM35-based thermostat with single threshold
            This sketch uses the analog value acquired on A0 to configure the setpoint

Created:	   2018-06-13
Last Modified: 2020-11-03

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

#define ADC_FSR	5.0 // ADC Full Scale Range [V]
#define Q		(ADC_FSR/1024.0) // ADC resolution [V/LSB]
#define K_LM35	Q/0.01 // LM35 instrument constant [°C/LSB]

#define Tmin	25.0		//	minimum temperature [°C]
#define Tmax	40.0		// maximum temperature [°C]
#define Vmin	0.0	
#define Vmax	ADC_FSR

#define m (double) (Tmax - Tmin)/(Vmax - Vmin)

double t, sp, dummy;

#define RELAY_OUTPUT		12	// Digital pin to control the output relay
#define LM35_ANALOG_IN		A0	// Analog pin to read the LM35 output
#define SETPOINT_ANALOG_IN  A1  // Analog pin to set the thermostat threshold

// Macros definition
#define FAN_OFF()    (digitalWrite(RELAY_OUTPUT, HIGH))
#define FAN_ON()     (digitalWrite(RELAY_OUTPUT, LOW))


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RELAY_OUTPUT, OUTPUT);
  FAN_OFF();
}

void loop() {
  sp = (analogRead(SETPOINT_ANALOG_IN)*Q - Vmin)*m + Tmin; // set threshold
  t = K_LM35 * analogRead(LM35_ANALOG_IN);
  //delay(100);
  t = K_LM35 * analogRead(LM35_ANALOG_IN); // measured temperature

  // Thermostat implementation
  (t > sp) ? FAN_ON() : FAN_OFF();

  // Communication
  Serial.print(sp);
  Serial.print(", ");
  Serial.print(t);
  Serial.print(", ");
  Serial.println(digitalRead(RELAY_OUTPUT) ? "OFF" : "ON");

  delay(100);
}
