//HERE I AM ASSIGNING EACH PIN TO ITS RESPECTIVE VARIABLE NAME
#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11
#define BUTTON_PIN 2

//HERE WE ARE KEEPING TRACK OF THE SEQUENCE
//THIS VARIABLE IS MARKED AS VOLATILE AS IT IS IN THE MAIN PROGRAM AND ISR
volatile int gCurrentSequence = 0;

//HERE WE ARE ASSIGNING FALSE TO THE VARIABLE isSequenceActive TO TOGGLE
//THE SEQUENCE FROM BEING ON AND OFF
//THIS VARIABLE IS VOLATILE AS IT WILL BE TOGGLED BY THE INTERRUPT
volatile bool gIsSequenceActive = false;

void setup() {
  // HERE WE ARE ASSIGNING THE LED PINS AS OUTPUTS
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // HERE WE ARE ASSIGNING THE BUTTON TO HAVE INPUT_PULLUP
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // HERE WE ARE SETTING UP THE INTERRUPT TO TRIGGER WHEN THE BUTTON GOES FROM HIGH TO LOW
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), toggleSequence, FALLING);
}

void loop() {
  // HERE IF THE isSequenceActive BOOL IS TRUE THEN THE SEQUENCE BEGINS
  if (gIsSequenceActive) {
    // EACH CASE FOLLOWS THE SEQUENCE FROM THE LAB
    switch (gCurrentSequence) {
      case 0: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, LOW); digitalWrite(BLUE_LED, LOW); break;  // R
      case 1: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, HIGH); digitalWrite(BLUE_LED, LOW); break;  // RG
      case 2: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, HIGH); digitalWrite(BLUE_LED, HIGH); break;  // RGB
      case 3: digitalWrite(RED_LED, LOW); digitalWrite(GREEN_LED, HIGH); digitalWrite(BLUE_LED, HIGH); break;   // GB
      case 4: digitalWrite(RED_LED, LOW); digitalWrite(GREEN_LED, LOW); digitalWrite(BLUE_LED, HIGH); break;    // B
      case 5: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, LOW); digitalWrite(BLUE_LED, HIGH); break;   // RB
    }

    //HERE WE ARE HAVING THE SEQUENCE LOOP AS IT ALWAYS STAYS BETWEEN 0 TO 5
    gCurrentSequence = (gCurrentSequence + 1) % 6;

    //HERE WE HAVE A SMALL DELAY TO HAPPEN BETWEEN CHANGES IN THE LED SEQUENCE
    delay(500);
  } else {
    //HERE IF SEQUENCE IS NOT ACTIVATED, THEN ALL LEDS OFF
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  }
}

//HERE THE ISR WILL TOGGLE THE SEQUENCE STATE EACH TIME THE BUTTON IS PRESSED
void toggleSequence() {
  gIsSequenceActive = !gIsSequenceActive;

  //HERE WE RESET THE SEQUENCE EVERYTIME THE SEQUENCE BEGINS 
  gCurrentSequence = 0;

}
