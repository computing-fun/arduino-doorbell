const uint8_t PIN_BUTTON = 11;
const uint8_t PIN_BELL = 8;
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

void setup() {
  // setup pin modes
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED_READY, OUTPUT);
  pinMode(PIN_LED_WAIT, OUTPUT);
  pinMode(PIN_BELL, OUTPUT);
}

void loop() {
  if (/*millis() - last_pressed >= DEBOUNCE && */digitalRead(PIN_BUTTON)) {
    last_pressed = millis();
    setLED(LED::Ready);
  }
  else {
    setLED(LED::Wait);
  }
}

