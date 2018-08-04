#include "Pitches.h"

// pins
const uint8_t PIN_BELL = 8;
const uint8_t PIN_BUTTON = 9;
const uint8_t PIN_LED_READY = 10;
const uint8_t PIN_LED_WAIT = 11;

// button
const unsigned long DEBOUNCE = 1000;
unsigned long last_pressed = 0;

// wait "timer"
const unsigned long WAIT_TIME = 5000;
unsigned long last_ring = 0;

// LED
unsigned long led_updated = 0;
const unsigned long LED_ON_TIME = 1000;

// LED states
enum LEDState {Off, Ready, Wait};

// current LED state
LEDState led_state = LEDState::Off;

// set the current set of the LED
void setLEDState(LEDState s) {
  led_state = s;
  switch (led_state)
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

// a tone is played at a pitch for a set time (duration)
struct Tone {
  int Pitch;
  int Duration;
};

// how many tones are in the melody
const int MELODY_LEN = 8;

// the bell's melody
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

// the current note being played. note_current is set to the end (MELODY_LEN) to stop bell from playing on start up
int note_current = MELODY_LEN;

// when the note was last updated
unsigned long note_updated = 0;

// arduino's setup function runs once before the loop.
// this is a good place to setup the pin modes.
void setup() {
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_BELL, OUTPUT);
  pinMode(PIN_LED_READY, OUTPUT);
  pinMode(PIN_LED_WAIT, OUTPUT);
}

// arduino's loop function runs over and over in a loop
void loop() {
  // if button is pressed and not being debounce
  if (digitalRead(PIN_BUTTON) && millis() - last_pressed > DEBOUNCE) {
    // button has been pressed so we update the last pressed to now
    last_pressed = millis();
    // if we are here in the code we know the button as been pressed but
    // now we want to see if it has been pressed before the wait "timer" is up.
    if (millis() - last_ring > WAIT_TIME) {
      // start bell by setting current note to the beginning (0)
      note_current = 0;
      // we started the ring so we need to update when it last rang to now
      last_ring = millis();
      // set the LED to its ready state
      setLEDState(LEDState::Ready);
    } else {
      // set the LED to its wait state
      setLEDState(LEDState::Wait);
    }
  }

  // if LED is both on and past the LED_ON_TIME, turn the LED off
  if (led_state != LEDState::Off && millis() - led_updated > LED_ON_TIME) {
    setLEDState(LEDState::Off);
  }

  // update bell
  // if current note is greater than the MELODY_LEN we know the melody is done and skip the update.
  // also check if we need to update the tone or wait for the current tone to finish.
  if (note_current < MELODY_LEN && millis() - note_updated > MELODY[note_current].Duration * 1.30) {
    // move to the next tone
    note_current++;
    // update with new tone
    tone(PIN_BELL, MELODY[note_current].Pitch, MELODY[note_current].Duration);
    // tone been updated so reset note_updated
    note_updated = millis();
  }
}

