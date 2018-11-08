#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include <io.h>
#include <interrupt.h>
#include <Arduino.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int main(void) {
	init();

	Serial.begin(9600);
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

	setup_IO();

	display.display();
	delay(500);

	display.clearDisplay();

	display.display();

	while (true) {
		if (Serial.available() >= 1) {
			Serial.println(BIN, Serial.read());
		}
	}
}

void setup_IO() {
	DDRD |= (1 << DDD4);		// Sets bit DDD4 to 1 within register DDRD
								// PD4 is now an output WOOT !!!
	DDRB |= (1 << DDB5);		// Onboard LED

	// Interrupt INT0 PD2
	DDRD &= ~(1 << DDD2);		// Clear pin
	PORTD |= (1 << PORTD2);		// Pull-up
	EICRA |= (1 << ISC00);
	EICRA |= (1 << ISC01);		// Rising edge
	EIMSK |= (1 << INT0);		// Turn on INT0

	// Interrupt INT1 PD3
	DDRD &= ~(1 << DDD3);		// CLear pin
	PORTD |= (1 << PORTD3);		// Pull-up
	//EICRA |= (1 << ISC10);
	EICRA |= (1 << ISC11);		// Falling edge
	EIMSK |= (1 << INT1);		// Turn on INT1
}

// Rising edge
ISR(INT0_vect) {
	
}

// Falling edge
ISR(INT1_vect) {
	
}