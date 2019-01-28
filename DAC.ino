#include<Wire.h>                   //Include Wire library for using I2C functions 
#include <LiquidCrystal.h>         //Include LCD library for using LCD display functions 

#define MCP4725 0x61              //MCP4725 address as 0x61 Change yours accordingly
LiquidCrystal lcd(2,3,4,5,6,7);   //Define LCD display pins RS,E,D4,D5,D6,D7
 
unsigned int adc;
byte buffer[3];                   

void setup() 
{
  Wire.begin();                    //Begins the I2C communication
  lcd.begin(16,2);                 //Sets LCD in 16X2 Mode
  lcd.print("CIRCUIT DIGEST");   
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Arduino");
  lcd.setCursor(0,1);
  lcd.print("DAC with MCP4725");
  delay(2000);
  lcd.clear();
  

}

void loop() 

{
  buffer[0] = 0b01000000;            //Sets the buffer0 with control byte (010-Sets in Write mode)
  adc = analogRead(A0) * 4;          //Read Analog value from pin A0 and convert into digital (0-1023) multiply with 4 gives (0-4096)
  
  float ipvolt = (5.0/4096.0)* adc;  //Finding voltage formula (A0)
  buffer[1] = adc >> 4;              //Puts the most significant bit values
  buffer[2] = adc << 4;              //Puts the Least significant bit values
  
 
  unsigned int analogread = analogRead(A1)*4 ; //Reads analog voltage from A1
  
  float opvolt = (5.0/4096.0)* analogread; //Finding Voltage Formula (A1)
  
  Wire.beginTransmission(MCP4725);         //Joins I2C bus with MCP4725 with 0x61 address
  
  Wire.write(buffer[0]);            //Sends the control byte to I2C 
  Wire.write(buffer[1]);            //Sends the MSB to I2C 
  Wire.write(buffer[2]);            //Sends the LSB to I2C
  
  Wire.endTransmission();           //Ends the transmission

  lcd.setCursor(0,0);     
  lcd.print("A IP:");
  lcd.print(adc);                   //Prints the ADC value from A0
  lcd.setCursor(10,0);
  lcd.print("V:");                  //Prints the Input Voltage at A0
  lcd.print(ipvolt);
  lcd.setCursor(0,1);
  lcd.print("D OP:");
  lcd.print(analogread);             //Prints the ADC value from A1 (From DAC)
  lcd.setCursor(10,1);
  lcd.print("V:");
  lcd.print(opvolt);                 //Prints the Input Voltage at A1 (From DAC)
   
  delay(500);
  lcd.clear();
}
