#include "Pitches.h"

/*
   Button
*/
const uint8_t PIN_BUTTON = 11;
const unsigned long DEBOUNCE = 1000;
unsigned long last_pressed = 0;

/*
   Anti spam waiting
*/
const unsigned long WAIT_TIME = 5000;
// last_ring is set to WAIT_TIME to stop the doorbell from waiting at start up
unsigned long last_ring = WAIT_TIME;

/*
   LED
*/
const uint8_t PIN_LED_READY = 13;
const uint8_t PIN_LED_WAIT = 12;
enum LEDType {Off, Ready, Wait};
LEDType led_type = LEDType::Off;
unsigned long led_updated = 0;
void setLED(LEDType type) {
  switch (led_type = type)
  {
    case Off:
      digitalWrite(PIN_LED_READY, LOW);
      digitalWrite(PIN_LED_WAIT, LOW);
      break;
    case Ready  :
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
const unsigned long LED_ON_TIME = 1000;

/*
   Bell
*/
const uint8_t PIN_BELL = 8;
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
   setup pin modes
*/
void setup() {
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED_READY, OUTPUT);
  pinMode(PIN_LED_WAIT, OUTPUT);
  pinMode(PIN_BELL, OUTPUT);
}

/*
   main update loop
*/
void loop() {
  // check button press
  if (millis() - last_pressed > DEBOUNCE && digitalRead(PIN_BUTTON)) {
    last_pressed = millis();
    // check if person is being impatient
    if (millis() - last_ring > WAIT_TIME) {
      last_ring = millis();
      // start bell by setting current note to the beginning.
      note_current = 0;
      setLED(LEDType::Ready);
    } else {
      setLED(LEDType::Wait);
    }
  }

  // turn off led after LED_ON_TIME
  if (led_type != LEDType::Off && millis() - led_updated > LED_ON_TIME) {
    setLED(LEDType::Off);
  }

  // update bell
  if (note_current < MELODY_LEN && millis() - note_updated > MELODY[note_current].Duration * 1.30) {
    tone(PIN_BELL, MELODY[note_current].Pitch, MELODY[note_current].Duration);
    note_updated = millis();
    note_current++;
  }
}

