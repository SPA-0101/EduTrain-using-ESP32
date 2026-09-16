[⬆️ Back to README](../README.md)


// 6_switches.ino
// Cleaned, expanded sketch for 6 switches with non-blocking debounce
// Uses INPUT_PULLUP: wiring should connect switches between pin and GND

const uint8_t NUM_BUTTONS = 8;
const uint8_t buttonPins[NUM_BUTTONS] = {23, 22, 1, 3, 21, 19, 18, 5}; // change pins as needed

// Extra button + LED
const uint8_t EXTRA_BUTTON_PIN = 36;
const uint8_t EXTRA_LED_PIN = 39;

// Two-way selector (SPDT-like): one side -> RED, other side -> GREEN
const uint8_t TW_RED_PIN = 34;    // reads LOW when selector is on RED side
const uint8_t TW_GREEN_PIN = 35; // reads LOW when selector is on GREEN side
const uint8_t RED_LED_PIN = 32;
const uint8_t GREEN_LED_PIN = 33;

// Debounce settings
const unsigned long debounceDelay = 50; // ms

// Per-button state tracking
int buttonState[NUM_BUTTONS];        // current stable state (HIGH or LOW)
int lastReading[NUM_BUTTONS];        // last raw reading from pin
unsigned long lastDebounceTime[NUM_BUTTONS]; // last time the reading changed

// Extra button state tracking
int extraButtonState;
int lastExtraReading;
unsigned long lastExtraDebounceTime;

// Two-way selector tracking
int twState = 0; // 0 = neither, 1 = RED, 2 = GREEN
int lastTwRedReading;
int lastTwGreenReading;
unsigned long lastTwDebounceTime;

void setup() {
  Serial.begin(9600);

  // Configure pins and initialize state arrays for the 6 buttons
  for (uint8_t i = 0; i < NUM_BUTTONS; ++i) {
    pinMode(buttonPins[i], INPUT_PULLUP); // using internal pull-up
    int r = digitalRead(buttonPins[i]);
    buttonState[i] = r;
    lastReading[i] = r;
    lastDebounceTime[i] = 0;
  }

  // Extra button + LED setup
  pinMode(EXTRA_BUTTON_PIN, INPUT_PULLUP);
  pinMode(EXTRA_LED_PIN, OUTPUT);
  int er = digitalRead(EXTRA_BUTTON_PIN);
  extraButtonState = er;
  lastExtraReading = er;
  lastExtraDebounceTime = 0;
  digitalWrite(EXTRA_LED_PIN, (extraButtonState == LOW) ? HIGH : LOW);

  // Two-way selector + LEDs setup
  pinMode(TW_RED_PIN, INPUT_PULLUP);
  pinMode(TW_GREEN_PIN, INPUT_PULLUP);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  lastTwRedReading = digitalRead(TW_RED_PIN);
  lastTwGreenReading = digitalRead(TW_GREEN_PIN);
  lastTwDebounceTime = 0;
  // Ensure LEDs initial state is off
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);

  Serial.println("Ready: 8-switch input + extra button LED + 2-way selector (RED/GREEN)");
}

void loop() {
  updateButtons();
  updateExtraButtonAndLed();
  updateTwoWaySwitch();
  // other code can run here without being blocked by debounce
}

void updateButtons() {
  unsigned long now = millis();

  for (uint8_t i = 0; i < NUM_BUTTONS; ++i) {
    int reading = digitalRead(buttonPins[i]);

    // If the reading changed, reset the debounce timer
    if (reading != lastReading[i]) {
      lastDebounceTime[i] = now;
    }

    // If the reading has been stable for longer than debounceDelay, take it as the actual state
    if ((now - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        // With INPUT_PULLUP: LOW means pressed, HIGH means released
        if (buttonState[i] == LOW) {
          // Pressed
          Serial.print("#S|SW(");
          Serial.print(i);
          Serial.println(")|[]#");
        }
      }
    }

    // Save the reading for the next loop
    lastReading[i] = reading;
  }
}

void updateExtraButtonAndLed() {
  unsigned long now = millis();
  int reading = digitalRead(EXTRA_BUTTON_PIN);

  if (reading != lastExtraReading) {
    lastExtraDebounceTime = now;
  }

  if ((now - lastExtraDebounceTime) > debounceDelay) {
    if (reading != extraButtonState) {
      extraButtonState = reading;
      // With INPUT_PULLUP: LOW means pressed
      if (extraButtonState == LOW) {
        digitalWrite(EXTRA_LED_PIN, HIGH); // turn LED on when pressed
      } else {
        digitalWrite(EXTRA_LED_PIN, LOW);  // turn LED off when released
      }
    }
  }

  lastExtraReading = reading;
}

void updateTwoWaySwitch() {
  unsigned long now = millis();
  int redReading = digitalRead(TW_RED_PIN);
  int greenReading = digitalRead(TW_GREEN_PIN);

  // If either reading changed, reset debounce timer
  if (redReading != lastTwRedReading || greenReading != lastTwGreenReading) {
    lastTwDebounceTime = now;
  }

  if ((now - lastTwDebounceTime) > debounceDelay) {
    // Determine new state
    int newState = 0;
    if (redReading == LOW && greenReading == HIGH) {
      newState = 1; // RED selected
    } else if (greenReading == LOW && redReading == HIGH) {
      newState = 2; // GREEN selected
    } else {
      // Neither side clearly selected (both HIGH or both LOW); treat as neither
      newState = 0;
    }

    if (newState != twState) {
      twState = newState;
      // Update LEDs to reflect selector
      if (twState == 1) {
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
      } else if (twState == 2) {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);        
      } else {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);        
      }
    }
  }

  lastTwRedReading = redReading;
  lastTwGreenReading = greenReading;
}

[⬆️ Back to README](../README.md)
