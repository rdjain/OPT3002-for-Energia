/* SDA P2_2
   SCL P2_1
   10k pullup resistors on I2C pins 
   Modified for Energia by Rishabh D Jain
   Original work - ClosedCube_OPT3002 by AA
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. 
 */
#include <Arduino.h>
#include<Wire.h>
#define RESULT 0x00
#define CONFIG 0x01
#define slaveAdr 0x44
uint16_t confData  = 0xC610;

void setup() {
  Serial.begin(9600);
  configureSensor(0xC610); //confiure sensor with configuration data c1
}

void loop() {
  Serial.println(readSensor());
  delay(200);   
  
}


uint16_t readRegister(uint8_t registerName){
  uint8_t lsb;
  uint8_t msb;
  uint16_t result;
  Wire.begin();
  Wire.beginTransmission(slaveAdr);
  Wire.write(registerName);
  Wire.endTransmission();
  Wire.requestFrom(slaveAdr, 2);
  while(Wire.available() < 2) {}
  msb = Wire.read();
  lsb = Wire.read();
  result = (msb << 8) | lsb;
  return result;
}

float readSensor(){
  uint16_t raw = readRegister(RESULT);
  //expo = (raw>>12 & 0x000F);
  //res = (raw & 0x0FFF);
  //intensity = (2^expo)*res*1.2 as per datasheet
  return(1.2*(1<<(raw>>12 & 0x000F))*(raw & 0x0FFF));//left shift does pow of 2
}

void configureSensor(uint16_t confData){
  uint16_t writeByte = confData;
  Wire.begin();
  Wire.beginTransmission(slaveAdr);
  Wire.write(CONFIG);
  Wire.write((unsigned char)(writeByte>>8));//write msb
  Wire.write((unsigned char)(writeByte&0x00FF));//write lsb
  Wire.endTransmission();
  Serial.print("Setup ");
  Serial.print(readRegister(CONFIG));
}



