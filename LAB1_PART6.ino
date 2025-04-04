//HERE I AM ASSIGNING EACH PIN TO ITS RESPECTIVE VARIABLE NAME
#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11
#define BUTTON_PIN 2


//HERE WE ARE ASSIGNING 0 TO THE BUTTONSTATE TO KEEP TRACK
//OF WHICH STATE THE BUTTON IS IN
int buttonState = 0;         

//HERE WE ARE KEEPING TRACK OF THE PREVIOUS STATE THAT THE BUTTON WAS IN
int lastButtonState = 0;    

//HERE WE ARE KEEPING TRACK OF THE SEQUENCE
int currentSequence = 0;     

//HERE WE ARE ASSIGNING FALSE THE VARIABLE isSequenceActive TO TOGGLE
//THE SEQUENCE FROM BEING ON AND OFF
bool isSequenceActive = false;

void setup() {
  // HERE WE ARE ASSIGING THE LED PINS AS OUTPUTS
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  //HERE WE ARE ASSIGING THE BUTTON TO HAVE INPUT_PULLUP
  //THIS WILL CAUSE THE BUTTON TO OUTPUT LOW WHEN PRESSED AND HIGH WHEN NOT
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  //HERE WE ARE CHECKING WHICH STATE THE BUTTON IS IN AND STORING IT TO THE
  //VARIABLE BUTTONSTATE
  buttonState = digitalRead(BUTTON_PIN);

  // HERE WE ARE WRITING A LINE THAT WILL HELP PREVENT SEEING A DELAY 
  //WHEN OUR LEDS TURN ON
  //THE CODE IS CHECKING IF THE CURRENT STATE OF THE BUTTON IS LOW (BUTTON PRESSED)
  // AND IF THE PREVIOUS STATE WAS HIGH (BUTTON NOT PRESSED)
  if (buttonState == LOW && lastButtonState == HIGH) {

    //THIS DELAY WILL HELP PREVENT US FROM SEEING THE DEBOUNCING OF THE BUTTON
    delay(50); 

    //IF THE IF STATE IS TRUE THEN IT WILL ENTER THE IF STATEMENT AND TOGGLE THE 
    //isSequenceActive VARIABLE TO BE NOTTED WHICH WILL CHANGE THE BOOL TO TRUE
    // THIS WILL THEN TURN ON THE LEDS AND BEGIN THE SEQUENCE
    isSequenceActive = !isSequenceActive; 

  //IF IT IS NOT THE CASE THAT THE BUTTON WAS PRESSED THE THE isSequenceActive BOOL
  //WILL REMAIN FALSE AND NOT TURN ON THE LEDS
    if (!isSequenceActive) {
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
    }
  }

  // IF THE IF STATEMENT WAS TRUE, THEN THE isSequenceActive BOOL
  //WILL NOW STATE TRUE AND BEGIN TO GO THRU THE SEQUENCE OF LEDS
  if (isSequenceActive) {
    //HERE EACH CASE IS WRITTEN TO FOLLOW THE SEQUENCE FROM THE LAB
    switch (currentSequence) {
      case 0: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, LOW); digitalWrite(BLUE_LED, LOW); break;  // R
      case 1: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, HIGH); digitalWrite(BLUE_LED, LOW); break;  // RG
      case 2: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, HIGH); digitalWrite(BLUE_LED, HIGH); break;  // RGB
      case 3: digitalWrite(RED_LED, LOW); digitalWrite(GREEN_LED, HIGH); digitalWrite(BLUE_LED, HIGH); break;   // GB
      case 4: digitalWrite(RED_LED, LOW); digitalWrite(GREEN_LED, LOW); digitalWrite(BLUE_LED, HIGH); break;    // B
      case 5: digitalWrite(RED_LED, HIGH); digitalWrite(GREEN_LED, LOW); digitalWrite(BLUE_LED, HIGH); break;   // RB
    }
    
    //THIS NEXT LINE OF CODE WILL MAKE THE CODE GO THRU THE SEQUENCE BY CAUSING IT 
    //TO ENTER EACH CASE AFTER INCREMENTING THE currentSequence VALUE BY 1
    //THE %6 IS A MODULUS OPERATOR THAT WILL KEEP THE RANGE BETWEEN 0-5
    currentSequence = (currentSequence + 1) % 6; 
    //SMALL DELAY BETWEEN THE CHANGES OF THE SEQUENCES
    delay(500);
  }

  //HERE WE ARE SAVING THE BUTTONSTATE AS THE LASTBUTTONSTATE BEFORE 
  //GOING THRU THE FOR LOOP AGAIN
  lastButtonState = buttonState;
}

