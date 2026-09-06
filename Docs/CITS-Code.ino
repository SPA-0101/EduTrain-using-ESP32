
// this constant won't change:
const int  buttonPin = 15;    // the pin that the pushbutton is attached to
const int  buttonPin1 = 4;
const int ledPin = 2;       // the pin that the LED is attached to

// Variables will change:

int buttonState = 0;         // current state of the button
int buttonState1 = 0;
int lastButtonState = 0;     // previous state of the button
int lastButtonState1 = 0;

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin1, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {

   sub_loop1();
   sub_loop2();
  
}

void sub_loop1() { 
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      
    } else {
      // if the current state is LOW then the button went from on to off:
          Serial.println("#S|TEST|[]#");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  
  // save the current state as the last state, for next time through the loop
 lastButtonState = buttonState;
   // etc etc etc
}

void sub_loop2() { 

 buttonState1 = digitalRead(buttonPin1);
  // compare the buttonState to its previous state
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
    if (buttonState1 == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      
    } else {
      // if the current state is LOW then the button went from on to off:
          Serial.println("#S|TEST1|[]#");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  
  lastButtonState1 = buttonState1;
  
   // etc etc etc
}


  
