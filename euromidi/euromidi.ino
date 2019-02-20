#include <MIDI.h>

#define POT_PIN    A0
#define LED_PIN    12
#define SWITCH_PIN 11
#define CLOCK1_PIN 10
#define CLOCK2_PIN  7
#define CLOCK4_PIN  6
#define CLOCK8_PIN  5
#define CLOCK16_PIN 4
#define CLOCK32_PIN 3
#define RESET_PIN   2

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
	pinMode(LED_PIN, OUTPUT);
	pinMode(SWITCH_PIN, OUTPUT);
	pinMode(CLOCK1_PIN, OUTPUT);
	pinMode(CLOCK2_PIN, OUTPUT);
	pinMode(CLOCK4_PIN, OUTPUT);
	pinMode(CLOCK8_PIN, OUTPUT);
	pinMode(CLOCK16_PIN, OUTPUT);
	pinMode(CLOCK32_PIN, OUTPUT);
	pinMode(RESET_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	digitalWrite(LED_BUILTIN, false);
	MIDI.begin();
	MIDI.setHandleClock(handleClock);
	MIDI.setHandleStart(handleStart);
	MIDI.setHandleStop(handleStop);
}

bool onoff = false;

unsigned long time = millis();

void loop() {
	digitalWrite(RESET_PIN, LOW);

	int mode = digitalRead(SWITCH_PIN);
	if (mode == HIGH) {
		if (MIDI.read()) {
			// buildin led is midi activity light
			onoff = !onoff;
			digitalWrite(LED_BUILTIN, onoff);
		}
	} else {
		int speed = analogRead(POT_PIN);
		int val = map(speed, 0, 1023, 1, 50);
		unsigned long newtime = time + val;
		if (millis() >= newtime) {
			handleClock();
			time = newtime;
		}
	}
}

int ticks = 0;
int counter = 0;

// run by default
bool running = true;

void handleClock(void) {
	if (!running)
		return;

	digitalWrite(CLOCK1_PIN, LOW);
	digitalWrite(CLOCK2_PIN, LOW);
	digitalWrite(CLOCK4_PIN, LOW);
	digitalWrite(CLOCK8_PIN, LOW);
	digitalWrite(CLOCK16_PIN, LOW);
	digitalWrite(CLOCK32_PIN, LOW);

	ticks++;
	digitalWrite(LED_PIN, LOW);
	if (ticks==6) {
		ticks = 0;
		digitalWrite(LED_PIN, HIGH);
		counter++;
		if (counter==64) {
			counter = 0;
		}

		digitalWrite(CLOCK1_PIN, HIGH);
		if (counter%2==0) {
			digitalWrite(CLOCK2_PIN, HIGH);
		}
		if (counter%4==0) {
			digitalWrite(CLOCK4_PIN, HIGH);
		}
		if (counter%8==0) {
			digitalWrite(CLOCK8_PIN, HIGH);
		}
		if (counter%16==0) {
			digitalWrite(CLOCK16_PIN, HIGH);
		}
		if (counter%32==0) {
			digitalWrite(CLOCK32_PIN, HIGH);
		}
	}
}

void handleStart(void) {
	running = true;
}

void handleStop(void) {
	running = false;
	ticks = 0;
	counter = 0;
	digitalWrite(RESET_PIN, HIGH);
	delay(1);
}
