#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include <io.h>
#include <interrupt.h>
#include <Arduino.h>

uint8_t displayNumber = 1;
#define displayFPS 1
#define displayROF 2
#define displayFPSp 3
#define displayROFp 4
#define displayAmmo 5
#define displayFPSROF 6


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

float FPS = 0;
int FPSm  = 0;
float ROF = 0;
int ROFm  = 0;
char msg_id;

uint16_t maxAmmo = 120;
uint16_t currentAmmo = 120;

String OK = "OK\r\n";
String input;
char chararray[50];
//char addressArray[12] = { "0","0","2","5","8","3","0","0","5","9","9","4" };
char addressArray[12] = { '0','0','2','5','8','3','0','0','5','9','9','4' };
char setupBluetoothReturn;

int main(void) {
	init();

	Serial.begin(9600);
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

	setup_IO();
	sei();

	display.display();
	delay(500);

	setupBluetoothReturn = setup_bluetooth();
	if (setupBluetoothReturn == 'f') {
		//No divices found
	} else {
		Serial.print("AT+CONN");
		Serial.print(setupBluetoothReturn);
		Serial.println();
	}

	delay(2000);

	Serial.println(Serial.readString());
	
	display.clearDisplay();

	displayMenu(displayNumber);

	display.display();

	while (true) {
		if (Serial.available() > 0) {
			switch (Serial.read()) {
			case 'F':
				FPS = Serial.parseFloat();
				if (FPS > 0) {
					Serial.println(FPS);
					if(currentAmmo > 0) currentAmmo--;
					displayMenu(displayNumber);
				}
				break;
			case 'R':
				ROF = Serial.parseFloat();
				Serial.println(ROF);
				break;
			}
		}
	}
}

char setup_bluetooth() {
	Serial.println("AT");			// Test connection
	
	serialWait();					// Wait for answer

	if (Serial.readString().equals("OK\r\n")) {
		Serial.println("AT+INQ");	// Search for devices
		delay(6000);
		serialWait();				// Wait for answer
		
		input = Serial.readString();// Store search result in String
		input.toCharArray(chararray, 50);// Convert Sting result to char Array
		
		// Print search result
		for (int i = 0; i < 50; i++) {
			Serial.print(chararray[i]);
		}
		Serial.println();

		// Check search result
		for (int i = 0; i < 50; i++) {
			if (chararray[i] == 'x') {
				for (int j = 0; j < 12; j++) {
					if (chararray[i + j + 1] == addressArray[j]) {
						return chararray[i - 3];
					} else {
						i += 12;
					}
				}
				return 'f';		// No devices found
			}
		}
	} else {
		return 'f';				// Did not respond with OK
	}
}

void serialWait() {
	while (Serial.available() == 0) {

	}
}

void displayMenu(uint8_t i) {
	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	switch (i) {
	case displayFPS:
		FPSm = FPS;
		display.print("FPS");
		if (FPSm < 1000) {
			display.setCursor(60, 10);
		} else {
			display.setCursor(20, 10);
		}
		display.setTextSize(3);
		display.print(FPSm);
		break;
	case displayROF:
		ROFm = ROF;
		display.print("ROF");
		display.setCursor(20, 10);
		display.setTextSize(3);
		display.print(ROFm);
		break;
	case displayFPSp:
		display.print("FPS+");
		display.setCursor(20, 10);
		display.setTextSize(3);
		display.print(FPS);
		break;
	case displayROFp:
		display.print("ROF+");
		display.setCursor(20, 10);
		display.setTextSize(3);
		display.print(ROF);
	case displayAmmo:
		display.print("AMMO");
		display.setCursor(100, 25);
		display.setTextSize(1);
		display.print("/");
		display.print(maxAmmo);
		display.setCursor(40, 10);
		display.setTextSize(3);
		display.print(currentAmmo);
	case displayFPSROF:
		FPSm = FPS;
		ROFm = ROF;
		display.print("FPS/ROF");
		display.setTextSize(2);
		display.setCursor(60, 0);
		display.print(FPSm);
		display.setCursor(60, 15);
		display.print(ROFm);
		display.setTextSize(1);
		display.setCursor(100, 10);
		display.print("FPS");
		display.setCursor(100, 20);
		display.print("ROF");
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
	//EICRA |= (1 << ISC00);
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
	//displayNumber--;
	//displayMenu(displayNumber);
}

// Falling edge
ISR(INT1_vect) {
	//displayNumber++;
	//displayMenu(displayNumber);
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