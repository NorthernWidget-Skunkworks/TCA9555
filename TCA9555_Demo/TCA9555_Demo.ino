//TCA9555_Demo.ino
#include <TCA9555.h>

TCA9555 IO(0x20); //Define instance of IO Expander (at default address)

void setup() {
	Serial.begin(9600); //Initialize serial for demo purposes only
	Serial.println("Welcome to the Expanded Machine...");  //Print ubiquitous initializer 

	IO.Begin();
	for(int i = 0; i < 16; i++) {
		IO.PinMode(i, INPUT); //Set all pins as inputs (should be by default, but make sure)
	}
}

void loop() {
	Serial.println("Pin Status:");

	for(int i = 0; i < 16; i++) {
		Serial.print(i); 
		Serial.print(" : ");
		Serial.println(IO.DigitalRead(i)); //Print state of the pin
	}

	delay(1000); //Wait a second
}