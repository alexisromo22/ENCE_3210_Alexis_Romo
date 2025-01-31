//here i am defining my pins
#define BUTTON1 3       
#define RED_LED 5       
#define GREEN_LED 4     


//the volatile being used in the next few lines is to ensure the variables
//are being properly updated inside ISRs

//this line will make the counter increase every 1hz 
//this counter is part of task 1
volatile unsigned long counter = 0;    

//this with the bool state will be used in order to make the greed led
//blink every 1hz as part of task 1
volatile bool greenLEDState = false;  

//this line with the bool state will be used to light the led
//whenever the button is pressed in order to complete task 2
volatile bool redLEDState = false;     

//this variable will be used in order to store the last time a message
//was printed 
unsigned long lastMessageTime = 0;

//this variable defines the 3 second interval that will be used when 
//printing to the serial monitor
const unsigned long messageInterval = 3000;  

//this line of code will use a bool state in order to
//determine when the button has been pressed
volatile bool buttonPressed = false;

void setup() {
  //these three lines of code are assigning the pin set up for each variable
  pinMode(BUTTON1, INPUT_PULLUP); 
  pinMode(RED_LED, OUTPUT);       
  pinMode(GREEN_LED, OUTPUT);     

  //this line is initializing the serial monitor
  Serial.begin(9600);

  // Initialize Timer1 for Task 1 (1 Hz)

  //the purpose of this line is to disable the interrupts
  //in order for the timer to not face any issues
  cli(); 
// the next few lines are used to create the 1 second timer
  //these lines will clear the registers of Timer1
  TCCR1A = 0; 
  TCCR1B = 0;

//this line initializes the counter
  TCNT1 = 0; 

  //OCR1A is set to 15625 in order to create the 1 second timer than runs at 1hz
  //this is done by using a prescaler of 1024
  //you must divide 16,000,000 by 1024, then subtract 1 in order to get this value
  OCR1A = 15624; 

//this line will enable CTC mode which clears the timer on compare match
  TCCR1B |= (1 << WGM12); 

//this line is setting the prescaler to 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);

  //this line will enable the Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A); 


//the next few lines are to create the second timer (task 2) that runs at 10hz
  //this line will clearing the resgisters of Timer2
  TCCR2A = 0; 
  TCCR2B = 0;
  TCNT2 = 0; //this line will iitialize the counter


//this line sets OCR2A to 249 in order to get a 10hz interrupt
//the math is calculated by dividing 16,000,000 by (64*(249+1))
  OCR2A = 249; 

  //this line will enable CTC mode which clears the timer on compare match
  TCCR2A |= (1 << WGM21);

  //the 16MHz clock will use a prescaler of 64
  TCCR2B |= (1 << CS22);  

  //this line will enable Timer2 compare interrupt
  TIMSK2 |= (1 << OCIE2A); 

// this line will enable global interrupts after the setup completes
  sei();  
}

void loop() {
  //this if statement is checking if 3 seconds have passed in order to print
  //to the serial moniter every 3 seconds
  if (millis() - lastMessageTime >= messageInterval) {
    //this line will update the "lastMessageTime"
    lastMessageTime = millis();

    ////the counter value will then be printed to the serial monitor
    Serial.print("Message: Counter value is ");
    Serial.println(counter);
  }

  // this if statement will check the state of the GREEN_LED
  //and turn it on or off depending on if it is high or low
  if (greenLEDState) {
    digitalWrite(GREEN_LED, HIGH);  
  } else {
    digitalWrite(GREEN_LED, LOW);   
  }

  // this if statement will check the state of the RED_LED
  //and turn it on or off depending on if it is high or low
  if (redLEDState) {
    digitalWrite(RED_LED, HIGH);    
  } else {
    digitalWrite(RED_LED, LOW);     
  }
}


//this section of the code will take care of completely task 1 (1Hz timer)
ISR(TIMER1_COMPA_vect) {
  //the counter will increase every second 
  counter++; 
  //the state of the led will then toggle in order to make the
  //led blink every 1 second (1Hz)
  greenLEDState = !greenLEDState; 
}


//this second of the code will complete task 2 (10Hz timer)
ISR(TIMER2_COMPA_vect) {
  //this line of code will check the state of the button 
  //in order to react to when the button has been pressed
  bool button1State = digitalRead(BUTTON1);


  //when the button is pressed, the state of the RED led will change to true
  //which will cause the RED led to turn on
  if (button1State == HIGH) {
    redLEDState = true;  
    //while the button is not pressed or after it has stopped being pressed
    //then the led will no longer be turned on
  } else {
    redLEDState = false; 
  }
}


