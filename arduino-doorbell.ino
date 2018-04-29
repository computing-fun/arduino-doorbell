#include "Pitches.h"

/*
   pins
*/
const uint8_t PIN_BELL = 8;
const uint8_t PIN_BUTTON = 9;
const uint8_t PIN_LED_READY = 10;
const uint8_t PIN_LED_WAIT = 11;

/*
   button
*/
const unsigned long DEBOUNCE = 1000;
unsigned long last_pressed = 0;

/*
   anti-spam waiting
*/
const unsigned long WAIT_TIME = 5000;
unsigned long last_ring = 0;

/*
   LED
*/
unsigned long led_updated = 0;
const unsigned long LED_ON_TIME = 1000;
enum LEDMode {Off, Ready, Wait};
LEDMode led_mode = LEDMode::Off;
void setLEDMode(LEDMode mode) {
  led_mode = mode;
  switch (led_mode)
  {
    case Off:
      digitalWrite(PIN_LED_READY, LOW);
      digitalWrite(PIN_LED_WAIT, LOW);
      break;
    case Ready:
      digitalWrite(PIN_LED_READY, HIGH);
      digitalWrite(PIN_LED_WAIT, LOW);
      break;
    case Wait:
      digitalWrite(PIN_LED_READY, LOW);
      digitalWrite(PIN_LED_WAIT, HIGH);
      break;
  }
  led_updated = millis();
}

/*
   bell
*/
struct Tone {
  int Pitch;
  int Duration;
};
const int MELODY_LEN = 8;
const Tone MELODY[MELODY_LEN] = {
  {NOTE_C4, 250},
  {NOTE_G3, 125},
  {NOTE_G3, 125},
  {NOTE_A3, 250},
  {NOTE_G3, 250},
  {0, 250},
  {NOTE_B3, 250},
  {NOTE_C4, 250}
};
// note_current is set to the end (MELODY_LEN) to stop bell from playing on start up
int note_current = MELODY_LEN;
unsigned long note_updated = 0;

/*
   setup pins
*/
void setup() {
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_BELL, OUTPUT);
  pinMode(PIN_LED_READY, OUTPUT);
  pinMode(PIN_LED_WAIT, OUTPUT);
}

/*
   main update loop
*/
void loop() {
  // is button press
  if (digitalRead(PIN_BUTTON) && millis() - last_pressed > DEBOUNCE) {
    last_pressed = millis();
    // is person is being impatient
    if (millis() - last_ring > WAIT_TIME) {
      last_ring = millis();
      // start bell by setting current note to the beginning.
      note_current = 0;
      setLEDMode(LEDMode::Ready);
    } else {
      setLEDMode(LEDMode::Wait);
    }
  }

  // turn off led after LED_ON_TIME
  if (led_mode != LEDMode::Off && millis() - led_updated > LED_ON_TIME) {
    setLEDMode(LEDMode::Off);
  }

  // update bell
  if (note_current < MELODY_LEN && millis() - note_updated > MELODY[note_current].Duration * 1.30) {
    tone(PIN_BELL, MELODY[note_current].Pitch, MELODY[note_current].Duration);
    note_updated = millis();
    note_current++;
  }
}

