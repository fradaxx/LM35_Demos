/*
Name:       LM35_Thermostat_without_Hysteresis.ino
Function:	Simple LM35-based thermostat with single threshold
            This sketch uses the analog value acquired on A0 to configure the setpoint

Created:	   2018-06-13
Last Modified: 2022-12-05

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

#define ADC_FSR	5.0 // ADC Full Scale Range [V]
#define Q		(ADC_FSR/1024.0) // ADC resolution (Quantization Step) [V/LSB]
#define K_LM35	Q/0.01 // LM35 instrument constant [°C/LSB]

#define TMIN	15.0		// setpoint minimum temperature [°C]
#define TMAX	40.0		// setpoint maximum temperature [°C]
#define VMIN	0.0         // Minimum voltage applied to the setpoint analog input	
#define VMAX	ADC_FSR     // Maximum voltage applied to the setpoint analog input

#define K_SP (double) (TMAX - TMIN)/1024 // scaling factor (from sp input voltage to temperature) [°C/LSB]

double t, sp, dummy;

#define HEATER_RELAY		12	// Digital pin to control the output relay
#define LM35_ANALOG_IN		A0	// Analog pin to read the LM35 output
#define SETPOINT_ANALOG_IN  A1  // Analog pin to set the thermostat threshold
#define DUMMY_ANALOG_IN		A2

// Definition of ON/OFF symbols to simplify relay module drive
// There are relays modules that are "active low" (i.e. they are ON on the LOW state of the driving digital input 
// and other that are "active high", i.e. they are ON on the HIGH status of the driving digital input
#define ON      LOW
#define OFF     HIGH

// Macros definition
#define HEATER_OFF()    (digitalWrite(HEATER_RELAY, OFF))
#define HEATER_ON()     (digitalWrite(HEATER_RELAY, ON))


void setup() {
  Serial.begin(115200);
  pinMode(HEATER_RELAY, OUTPUT);
  HEATER_OFF();
  Serial.println("LM35 Thermostat wo/ histeresys");
  Serial.println("setpoint temperature relay");
}

void loop() {
  sp = TMIN + K_SP * analogRead(SETPOINT_ANALOG_IN); // actual setpoint in degrees Celsius
  __attribute__((used)) dummy = analogRead(DUMMY_ANALOG_IN);
  t = K_LM35 * analogRead(LM35_ANALOG_IN); // actual temperature in degrees Celsius

  // Thermostat implementation
  (t > sp) ? HEATER_OFF() : HEATER_ON();

  // Communication
  Serial.print(sp);
  Serial.print(", ");
  Serial.print(t);
  Serial.print(", ");
  Serial.println(digitalRead(HEATER_RELAY) ? "0" : "1");

  delay(100);
}
