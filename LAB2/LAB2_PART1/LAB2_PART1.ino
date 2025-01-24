//HERE I AM ASSIGING THE VARIABLE TO EACH PIN
#define BUTTON1     2
#define BUTTON2     3
#define RED_LED     4    
#define GREEN_LED   5 
#define BLUE_LED    6   

//HERE I AM SETTING UP MY GLOBAL VARIABLES
//THESE ARE VOLATILE VALUES AS THEY WILL UPDATED BASED ON BUTTON PRESSED
volatile int button1Count = 0;
volatile int button2Count = 0;

void setup() {

 //HERE WE ARE ASSIGING INPUT_PULLUP TO THE BUTTON
 //WHICH WILL MAKE THE BUTTONS ACT AS PULL RESISTORS 
 //IN ORDER TO HELP REDUCE DEBOUNCING 
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

 //HERE I AM ASSIGNING MY LEDS AS OUTPUTS
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);


//HERE I AM SETTING UP MY INTERRUPTS THAT WILL BE CALLED WHENEVER 
//THE BUTTON CHANGES FROM HIGH TO LOW, THIS IS THE CASE FOR BOTH BUTTONS
  attachInterrupt(digitalPinToInterrupt(BUTTON1), button1ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), button2ISR, FALLING);

 //HERE WE ARE INITIALZING THE SERIAL COMMUNICATION
  Serial.begin(9600);
}

void loop() {
//HERE WE ARE CALCULATING THE DIFFERENCE BETWEEN HOW MANY TIMES EACH BUTTON HAS BEEN CLICKED
  int difference = button1Count - button2Count;

//HERE WE ARE WRITING CODE THAT WILL PRINT THE DIFFERENT TO OUR SERIAL MONITOR
  Serial.print("Difference: ");
  Serial.println(difference);


//HERE WE ARE SETTING UP IF STATEMENTS FOR WHENEVER ONE BUTTON HAS BEEN PRESSED
//MORE THAN THE OTHER
  if (button1Count > button2Count) {
    //IN THIS CASE WHEN BUTTON1 HAS BEEN PRESSED MORE THE REDLED WILL TURN ON, WHILE GREENLED IS OFF
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

  //IN THIS CASE WHEN BUTTON2 HAS BEEN PRESSED MORE THE GREENLED WILL TURN ON, WHILE REDLED IS OFF
  } else if (button1Count < button2Count) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);

  //IN THE CASE THAT BOTH BUTTONS HAVE BEEN PRESSED THE SAME NUMBER OF TIME
  //THEN BOTH LEDS WILL BE ON
  } else {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    
  }

//SMALL DELAY TO AVIOD EFFECTING RESULTS OUTPUTTED ON SERIAL MONITOR
  delay(150);
}



//HERE WE ARE CALLING THE FUNCTION FOR THE IRSR FOR BUTTON1 
//THAT WILL INCREASE BY ONE EVERY TIME THE BUTTON IS PRESSED
//THIS IS THE SAME CASE FOR BUTTON2ISR
void button1ISR() {
  button1Count++;
}

//IRSR FOR BUTTON2
void button2ISR() {
  button2Count++;
}
