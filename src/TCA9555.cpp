#include <Arduino.h>
#include <TCA9555.h>
#include <Wire.h>

TCA9555::TCA9555(int _ADR)
{
  ADR = _ADR; 
  Wire.begin();  
}


int TCA9555::Begin(void)
{
  Wire.beginTransmission(ADR); //Test if device present 
  if(Wire.endTransmission() != 0) return -1;
  else return 1;
}


int TCA9555::PinMode(int Pin, boolean PinType)
{
  if(Pin > 16 || Pin < 0)
  {
    return -1;  //Fail if pin out of range
  }

  if(PinType == 1)  //Set as INPUT
  {
    PinModeConf = PinModeConf | (0x01 << Pin);
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    SetDirection(PinModeConf);
    return 1;
  }
  else if(PinType == 0)  //Set as OUTPUT
  {
    PinModeConf = PinModeConf & ~(0x01 << Pin);
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(Pin); //DEBUG!
    // Serial.println(~(0x01 << Pin)); //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    SetDirection(PinModeConf);
    return 0;
  }
  else 
    return -1; //Fail if pin type not defined 
}

int TCA9555::DigitalWrite(int Pin, boolean State)
{
  // Serial.println("BANG!2");
  if(Pin > 16 || Pin < 0 || GetDirection(Pin) != 0)  
  {
    return -1; //Fail if pin out of range, or pin configured as input
  }

  if(State == HIGH)
  {
    Port = Port | (0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    // Serial.println(SetPort(Port)); //DEBUG!
    return 1;
  }
  else if(State == LOW)
  {
    Port = Port & ~(0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    // Serial.println(SetPort(Port)); //DEBUG!
    return 0;
  }
  else 
    return -1; //Fail if state is ill-defined
}

bool TCA9555::DigitalRead(int Pin) 
{
  //DEBUG!
  // if(Pin > 16 || Pin < 0 || GetDirection(Pin) != 1)  
  // {
  //   return -1; //Fail if pin out of range, or pin configured as output
  // }

  return (ReadWord(READ_REG) >> Pin) & 0x01; //Return specific pin state from word of pin values
}

int TCA9555::SetPort(uint16_t Config) {
  return WriteWord(Config, OUTPUT_REG); //Write configuration to the output register, return I2C status
}

int TCA9555::SetDirection(uint16_t Config) {
  return WriteWord(Config, CONFIG_REG); //Write configuration to the direction register, return I2C status
}

int TCA9555::SetPolarity(uint16_t Config) {
  return WriteWord(Config, POLARITY_REG); //Write configuration to the polarity register, return I2C status
}

bool TCA9555::GetDirection(uint8_t Pin) {  //Returns polairty of given pin
  uint16_t Polarity = 0; //Value to store polarity registers
  // Serial.println("BANG!0"); //DEBUG!
  Wire.beginTransmission(ADR);
  Wire.write(POLARITY_REG); 
  Wire.endTransmission();

  Wire.requestFrom(ADR, 2);
  Polarity = Wire.read(); //Read in low byte
  Polarity = Polarity + Wire.read() << 8; //Read in upper byte
  // Serial.println("BANG!1"); //DEBUG!
  return (Polarity >> Pin) & 1; //Return value of Pin index
}

//IN DEVELOPMENT
int TCA9555::ReadPort(uint16_t Config) {
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x01);        //Send to port configuration register
  Wire.write(Config);              //Set port 1-4 as OUTPUT, all others as inputs
  return Wire.endTransmission();    // stop transmitting
}

int TCA9555::WriteWord(uint16_t Val, uint8_t Reg) { //Write word to location, starting at Reg 
  Wire.beginTransmission(ADR);
  Wire.write(Reg);
  Wire.write(Val & 0xFF); //Write low byte
  Wire.write(Val >> 8); //Write high byte
  return Wire.endTransmission(); //return I2C write status 
}

uint16_t TCA9555::ReadWord(uint8_t Reg) //Read word, starting at point Reg
{
  uint16_t Val = 0; //Value to store word read from registers
  Wire.beginTransmission(ADR);
  Wire.write(Reg); //Provide starting position
  Wire.endTransmission();

  Wire.requestFrom(ADR, 2); //Request 2 bytes (1 word) from device
  Val = Wire.read(); //Read in low byte
  Val = Val | (Wire.read() << 8); //Read in high byte
  return Val; //Return word
}





