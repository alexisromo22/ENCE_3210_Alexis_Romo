//HERE I AM ASSIGNING MY VARIABLE TO A PIN
#define BUTTON_PIN 2 

/HERE I AM ASSIGNING MY GLOBAL ARRAYS THAT WILL BE USED FOR CALCULATING
volatile int x[10] = {5, 8, 10, 15, 20, 25, 30, 35, 40, 45}; 

//HERE I AM CREATING MY EMPTY ARRAY WHERE THE RESULTS WILL BE PUT INTO AFTER CALCULATING
volatile int y[10] = {0}; 


void setup() {
  //HERE I AM INITIALIZING SERIAL COMMUNICATION
  Serial.begin(9600);

  //HERE I AM SETTING MY BUTTON TO ACT AS A PULL RESISTOR
  //THE PIN WILL READ HIGH WHEN NOT PRESSED AND LOW WHEN PRESSED
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //HERE I AM SETTING UP MY INTERRUPT TO CALL calculateY WHEN THE BUTTON IS PRESSED
  //THE FALLING MEANS IT WILL BE FALLING EDGE DETECTED
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), calculateY, FALLING);
}

void loop() {
  //HERE WE ARE PRINTING THE ARRAYS
  delay(1000);
  printArrays();
}

// HERE WE ARE CREATING OUR calculateY function
void calculateY() {

  //HERE WE ARE CALLING FOR noInterrupts TO MAKE SURE THERE IS NO
  //INTERFERENCE WITH THE CALCULATIONS
  noInterrupts(); 

  //HERE WE ARE CALCULATING FOR THE EQUATION y[n] = 2 * x[n] - x[n - 1]
  //THE FOR LOOP WILL TAKE INTO ACCOUNT EACH VALUE IN THE ARRAY
  for (int n = 0; n < 10; n++) {
    //THIS FIRST CONDITION IS TO HANDLE THE FIRST NUMBER IN THE ARRAY AS
    //THERE IS NO PREVIOUS NUMBER IN THE ARRAY WHEN INSERTING THE FIRST VALUE INTO
    //THE EQUATION
    if (n == 0) {
      y[n] = 2 * x[n]; 
    } else {
      //ALL OTHER CASES WILL GO THROUGH HERE THEN BE INSERTED INTO THE Y ARRAY
      y[n] = 2 * x[n] - x[n - 1];
    }
  }

  //HERE WE ALLOW ALL INTERRUPTS TO START AGAIN
  interrupts(); 
}

void printArrays() {
  //HERE WE ARE OUTPUTTING THE X ARRAY ONTO THE SERIAL MONITOR
  Serial.print("Array x: ");
  for (int i = 0; i < 10; i++) {
    Serial.print(x[i]);
    Serial.print(" ");
  }
  Serial.println();

  ////HERE WE ARE OUTPUTTING THE Y ARRAY ONTO THE SERIAL MONITOR
  Serial.print("Array y: ");
  for (int i = 0; i < 10; i++) {
    Serial.print(y[i]);
    Serial.print(" ");
  }
  Serial.println();
}
