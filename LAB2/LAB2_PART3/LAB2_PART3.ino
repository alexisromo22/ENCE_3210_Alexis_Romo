//HERE I AM NAMING ALL MY PIN ASSIGNMENTS
#define BUTTON1PIN   2
#define BUTTON2PIN   3
#define REDLEDPIN    4
#define GREENLEDPIN  5
#define BLUELEDPIN   6

//HERE I AM ASSIGNING A VARIABLE THAT WILL TRACK HOW MANY TIMES BUTTON HAS BEEN PRESSED
volatile int gPressCount = 0;    

//HERE I AM ASSIGNING A VARIABLE THAT WILL FLAG WHEN THE CALCULATION SHOULD START
volatile bool gIsCalculating = false; 

//HERE I AM ASSIGING A VARIABLE THAT WILL STORE WHICH OPERATION WILL
//BE APPLIED WHEN CALCULATING
int operationCounter = 0; 

//HERE I AM CREATING MY ARRAYS THAT WILL BE USED WHEN CALCULATING
int array1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int array2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

//HERE I AM CREATING THE ARRAY IN WHICH THE CALCULATION WILL BE STORED IN
int resultArray[10] = {};

// HERE THE ISR WILL TRIGGER ONCE BUTTON1 HAS BEEN PRESSED
void handleButton1Press() {
  //THE PRESS COUNT INCREASES EACH TIME THE BUTTON IS PRESSED
  gPressCount++;  
  if (gPressCount > 3) {  
    //IF THE BUTTON NUMBER IS GREATER THAN THREE THEN IT WILL RESET TO 1
    //THIS WAY THE LOOP IS ALWAYS 1-3.
    gPressCount = 1;
  }

  //HERE THE TIMES THE BUTTON HAS BEEN PRESSED WILL DETERMINE WHICH LED TURNS ON
  //AS WELL AS WHICH OPERATION WILL BE USED WHEN CALCULATING
  if (gPressCount == 1) {
    operationCounter = 1; // Addition
    digitalWrite(REDLEDPIN, HIGH);
    digitalWrite(GREENLEDPIN, LOW);
    digitalWrite(BLUELEDPIN, LOW);
  } else if (gPressCount == 2) {
    operationCounter = 2; // Subtraction
    digitalWrite(REDLEDPIN, LOW);
    digitalWrite(GREENLEDPIN, HIGH);
    digitalWrite(BLUELEDPIN, LOW);
  } else if (gPressCount == 3) {
    operationCounter = 3; // Multiplication
    digitalWrite(REDLEDPIN, LOW);
    digitalWrite(GREENLEDPIN, LOW);
    digitalWrite(BLUELEDPIN, HIGH);
  }
}

//THIS IS THE ISR FOR THE SECOND BUTTON
void handleButton2Press() {
  //THIS BOOL WILL BECOME TRUE TO FLAG THAT IS HAS STARTED TO CALCULATE
  gIsCalculating = true; 
}

void setup() {
  //HERE THE BUTTONS WILL USE PULL UP RESISTORS
  pinMode(BUTTON1PIN, INPUT_PULLUP);
  pinMode(BUTTON2PIN, INPUT_PULLUP);

  // HERE I AM SETTING THE LEDS TO ACT AS OUTPUTS
  pinMode(REDLEDPIN, OUTPUT);
  pinMode(GREENLEDPIN, OUTPUT);
  pinMode(BLUELEDPIN, OUTPUT);

  // HERE I AM STARTING ALL THE LEDS OFF
  digitalWrite(REDLEDPIN, LOW);
  digitalWrite(GREENLEDPIN, LOW);
  digitalWrite(BLUELEDPIN, LOW);

  //HERE WE ARE ATTACHING THE INTERRUPTS TO THE BUTTONS WHEN THEY GO FROM HIGH TO LOW
  attachInterrupt(digitalPinToInterrupt(BUTTON1PIN), handleButton1Press, FALLING); // Trigger on button press
  attachInterrupt(digitalPinToInterrupt(BUTTON2PIN), handleButton2Press, FALLING);

  //HERE WE ARE INITIALIZING SERIAL COMMUNICATION 
  Serial.begin(9600);
}

void loop() {
  // HERE WE ARE CHECKING IF THE gISCALCULATING HAS BEEN TRIGGERED
  if (gIsCalculating) {
    // IF IT IS, THEN ALL LEDS WILL TURN OFF TO INDICATE CALCULATING
    digitalWrite(REDLEDPIN, LOW);
    digitalWrite(GREENLEDPIN, LOW);
    digitalWrite(BLUELEDPIN, LOW);

  //HERE THE FOR LOOP WILL GO THRU ALL 10 ELEMENTS IN EACH ARRAY 
  //AND DO THE OPERATION PREVIOUSLY SELECTED TO EACH ELEMENT IN THE ARRAY 
  //IN THE ORDER EACH ELEMENT IS PLACED IN THE ARRAY
    for (int i = 0; i < 10; i++) {
      if (operationCounter == 1) {
        resultArray[i] = array1[i] + array2[i]; // Addition
      } else if (operationCounter == 2) {
        resultArray[i] = array1[i] - array2[i]; // Subtraction
      } else if (operationCounter == 3) {
        resultArray[i] = array1[i] * array2[i]; // Multiplication
      }

    //HERE WE ARE PRINTING THE RESULTS AND ADDING THEM TO THE EMPTY ARRAY
      Serial.print("Result [");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(resultArray[i]);
    }

    //HERE WE ARE TURNING ON ALL LEDS TO INDICATE THE RESULTS ARE READY
    digitalWrite(REDLEDPIN, HIGH);
    digitalWrite(GREENLEDPIN, HIGH);
    digitalWrite(BLUELEDPIN, HIGH);

    //HERE WE HAVE A LITTLE DELAY FOR THE USER TO SEE 
    //THE LEDS ALL ON TO INDICATE COMPLETION BEFORE 
    //RESETTING TO PICKING BETWEEN THE OPERATORS
    delay(2500);
    digitalWrite(REDLEDPIN, LOW);
    digitalWrite(GREENLEDPIN, LOW);
    digitalWrite(BLUELEDPIN, LOW);

    //HERE WE RESET THE BOOL AS WE HAVE COMPLETED CALCULATED
    gIsCalculating = false;
  }
}