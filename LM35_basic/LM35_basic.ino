/*
Name:			LM35_basic.ino
Function:		Simple LM35-based thermometer

Created:		2018-06-12 11:46:32
Last Modified:	2020-11-03

Copyright(c) 2018, Francesco Adamo - Polytechnic University of Bari - Italy
e-mail: francesco.adamo@poliba.it

Permission is hereby granted, free of charge, to any person
obtaining a copy of this softwareand associated documentation
files(the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions :

The above copyright noticeand this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

*/

#define ADC_FSR		(double) 1.1	// Voltage reference for analog input
#define K_LM35		(double) (ADC_FSR/1024)/0.01 // transducer constant [°C/LSB]
#define	LM35_AIN	A0	// Analog input pin


void setup()
{
	Serial.begin(115200);
	analogReference(INTERNAL); // Switch the ADC reference input to the 1.1 V internal reference
	Serial.print("LM35 Thermometer resolution: ");
	Serial.print(K_LM35);
	Serial.println(" degC/LSB");
}

void loop()
{
	Serial.print("t = ");
	Serial.print((float) K_LM35*analogRead(LM35_AIN));
	Serial.println(" degC");
	delay(100);
}
