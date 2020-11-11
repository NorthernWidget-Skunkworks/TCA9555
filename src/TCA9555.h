#ifndef TCA9555_h
#define TCA9555_h

#include "Arduino.h"

//Note: All base registers corespond to the 0th register in the pair
#define READ_REG 0x00  //Define input register base (0x00 ~ 0x01)
#define OUTPUT_REG 0x02  //Define output register base (0x02 ~ 0x03)
#define POLARITY_REG 0x04 //Define polarity inversion register base (0x04 ~ 0x05)
#define CONFIG_REG 0x06 //Define configuration (input vs output) register base (0x06 ~ 0x07)

class TCA9555
{
    //     enum IO_Type
    // {
    //     INPUT,
    //     OUTPUT
    // };

  public:
    TCA9555(int _ADR);
    TCA9555(); 
    int Begin(int _ADR); 
    int Begin(void); 
    int PinMode(int Pin, boolean PinType);
    int DigitalWrite(int Pin, boolean State);
    bool DigitalRead(int Pin);

  private:
    int ADR;
  	uint16_t PinModeConf = 0xFFFF; //All pins natively inputs
    uint16_t Port = 0x0000; //All pins natively off
    uint16_t PinPolarityConfig = 0x0000; //All pins natively non-inverted

    int SetPort(uint16_t Config);
    int SetDirection(uint16_t Config);
    int SetPolarity(uint16_t Config);
    bool GetDirection(uint8_t Pin);
    int ReadPort(uint16_t Config); //IN DEVELOPMENT
    //Helper functions
    int WriteWord(uint16_t Val, uint8_t Reg);
    uint16_t ReadWord(uint8_t Reg);
};

#endif