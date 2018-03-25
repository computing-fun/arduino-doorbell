#include "Pitches.h"

const uint8_t PIN_BUTTON = 11;
const unsigned long DEBOUNCE = 1000;
unsigned long last_pressed = 0;

const uint8_t PIN_LED_READY = 13;
const uint8_t PIN_LED_WAIT = 12;
enum LED {Off, Ready, Wait};

void setLED(LED led) {
  switch (led)
  {
    case Off:
      digitalWrite(PIN_LED_READY, LOW);
      digitalWrite(PIN_LED_WAIT, LOW);
    case Ready  :
      digitalWrite(PIN_LED_READY, HIGH);
      digitalWrite(PIN_LED_WAIT, LOW);
      break;
    case Wait:
      digitalWrite(PIN_LED_READY, LOW);
      digitalWrite(PIN_LED_WAIT, HIGH);
      break;
  }
}

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
int note_current = 0;
unsigned long note_updated = 0;

void setup() {
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED_READY, OUTPUT);
  pinMode(PIN_LED_WAIT, OUTPUT);
  pinMode(PIN_BELL, OUTPUT);
}

void loop() {
  if (millis() - last_pressed > DEBOUNCE && digitalRead(PIN_BUTTON)) {
    last_pressed = millis();
    note_current = 0;
  }

  if (note_current < MELODY_LEN && millis() - note_updated > MELODY[note_current].Duration * 1.30) {
    tone(PIN_BELL, MELODY[note_current].Pitch, MELODY[note_current].Duration);
    note_updated = millis();
    note_current++;
  }
}

