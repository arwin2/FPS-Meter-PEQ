#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include <io.h>
#include <interrupt.h>
#include <Arduino.h>

uint8_t displayNumber = 1;
#define displayFPS 1
#define displayROF 2

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

float FPS;
int ROF;
char msg_id;

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
		if (Serial.available() > 0) {
			switch (Serial.read()) {
			case 'F':
				FPS = Serial.parseFloat();
				if (FPS > 0) {
					Serial.println(FPS);
					displayMenu(displayNumber);
				}
				break;
			case 'R':
				ROF = Serial.parseInt();
				Serial.println(ROF);
				break;
			}
		}
	}
}

void displayMenu(uint8_t i) {
	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	switch (i) {
	case displayFPS:
		display.print("FPS");
		display.setCursor(20, 10);
		display.setTextSize(3);
		display.print(FPS);
		break;
	case displayROF:
		break;
	}
	display.display();
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




/*if (Serial.read() == 'F') {
	for (uint8_t i = 0; i < 6; i++) {
		FPS[i] = Serial.read();
	}
	for (uint8_t i = 0; i < 6; i++) {
		Serial.print(FPS[i]);
	}
	Serial.println();
}
else if (Serial.read() == 'R') {
	for (uint8_t i = 0; i < 3; i++) {
		ROF[i] = Serial.read();
	}
}*/