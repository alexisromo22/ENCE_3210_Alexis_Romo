//Here i am including all my libraries that I will use in my project

//these libraries are for the temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

//these libraries are for the OLED screen being used in the project
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <I2Cdev.h>

//this library is used for the gyroscope
#include <MPU6050.h>

//this is the file with the bitmap of the images that are to be displayed
//on the OLED screen
#include "images.h"

//here we are assigning each variable to a pin
#define LED           9
#define LIGHT         5
#define TEMP_SENSOR   8

//this pin was suppose to be set to pin 6 as button 6 is the lock button
//but the calibration button had to be used instead as pin6 was not supporting interrupts
#define LOCK          3
#define UNITS         4
#define ONOFF         2

//here we are defining the width and height of the screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

//here we are defining the reset pin (-1 means no reset pin is used)
#define OLED_RESET    -1

//here we are setting the I2C address of the screen
#define SCREEN_ADDRESS 0x3C

//here we are setting the timer1 compare value to 62500 which is 16MHz / 256 prescaler for Timer 1
#define TIMER1_COMPARE_VALUE 62500  
//here we are setting a timer1 max to check if the 90 seconds of inactiviy have passed
#define TIMER1_COUNTER_MAX   90  


//here we are setting timer2 compare value to 250 in order to trigger interrupts every 4ms
#define TIMER2_COMPARE_VALUE 250 // 4 ms

//here we are setting the timer2 max to 3 seconds as we want the units button
//to change the units after it has been pressed for three seconds
#define TIMER2_COUNTER_MAX   750 // 3 Seconds

//here we are creating an instance of the screen using &Wire (I2C) with the screen width and height
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//here we are creating an instance of the onewire library to communicate with the temp sensor
OneWire oneWire(TEMP_SENSOR);

//here we are creating an instance of the dallas temperature library to interference with the temp sensor
DallasTemperature sensors(&oneWire);

//here we are creating an instance of th e MPU6050 sensor inorder to read the gyroscrope data
//to help us reorienting the screen when needed
MPU6050 accelgyro;  // MPU6050 object for reading accelerometer and gyro data

//here we are inlizing the readings of the accelermeter and gyroscope
int16_t ax, ay, az;  // Accelerometer raw readings
int16_t gx, gy, gz;  // Gyroscope raw readings

int rotation = 0;  // Rotation state: 0 = landscape, 2 = upside-down portrait


//here we are setting float values for temperature in F and C to be stored 
//when system is locked
float tempF = 0;
float tempC =0;

//this variable is being used to change between 0 and 1
//when set to 0, displaying temperature should be F, and C when set to 1.
volatile int gFtemp = 0;

//here are setting a variable that will keep count of the inactivity within the system
volatile int inactivityCounter = 0;

//here we are setting a bool for when the screen needs to be locked or unlocked
volatile bool freezeScreen = false;

//here we are setting a bool for when the system should be on or off
volatile bool turnOff = false;

//here we are setting a counter for timer2 when changing temperature units after
//button has been pressed for three seconds
int gCounterTimer2 = 0;

//here we are setting x to 0
//this is because we want the LED to turn on when the system is on
//However, when the system is off x will be set to 1 which will stop the light button
//from working. This is because the Light button is only read when x is less than 1.
//When system is back on, x will be reset to 0 and light button will function as normal
int x = 0;



void setup() {
  //here we are initilizing serial communication to read information from our serial monitor
  Serial.begin(9600);

  //this if statement initilizes the screen to make sure it is working, or else it
  //will print failed messages
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  //here we are clearing the display and updating it afterwards
  display.clearDisplay();
  display.display();

  //here we are setting our inputs and outputs and begining our sensors
  pinMode(LIGHT, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(UNITS, INPUT);
  sensors.begin();

  //here we are initializing the MPU6050 sensor then checking if it successfully connects
  accelgyro.initialize();
  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }

  //here we have our two interrupts, one for our lock and another for the onOff button
  attachInterrupt(digitalPinToInterrupt(LOCK), lockButtonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ONOFF), onOffButtonISR, RISING);

  //here we are disabling our Interrupts to set up our timers
  noInterrupts();
  TCCR1A = 0;  // Normal mode
  TCCR1B = 0;
  TCNT1 = 0;  
  OCR1A = TIMER1_COMPARE_VALUE;  // Set compare match register for 1-second interrupt
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS12);  // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // Enable Timer1 compare interrupt


  // Init Timer 2 (8 bit)
  // Speed of Timer1 = 16MHz/256 = 62.5kHz (1.6us)
  TCCR2A = 0;
  TCCR2B = 0;
  OCR2A = TIMER2_COMPARE_VALUE;
  TCCR2B |= (1<<WGM21); // CTC Mode

  // Start Timer
  // This will start when CAL Button is pressed
  TCCR2B |= (1<<CS22) | (1<<CS21); // 256 prescaler
  TIMSK2 |= (1<<OCIE2A); 
  //here we enable Interrupts
  interrupts();


}

void loop() {

  //here if in the system is on, then the light button will be 
  //digitally read to check if it is HIGH
  //if read as HIGH then the LED will be on, otherwise off
  //The light button seems to need to be pressed for 2-3 seconds
  //in order to turn on. However, when an interrupt service is on the button will function
  //without needed to be pressed for a few seconds
  if (x < 1) {
  if (digitalRead(LIGHT) == HIGH) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  }

  //here if the UNITS button is pressed and read as HIGH then Timer 2 will start
  //so that the units can switch after 3 seconds
    if (digitalRead(UNITS) == HIGH)
  {
    // Start Timer 2
    TCCR2B |= (1<<CS21) | (1<<CS22);
  }

  //this if statement is used to detected the inactivity, as long as one of the buttons
  //is pressed then the inactivity counter will reset to 0 to prevent the system from turning off
  if (digitalRead(LIGHT) == HIGH || digitalRead(UNITS) == HIGH || digitalRead(LOCK) == HIGH) {
    inactivityCounter = 0;  // Reset inactivity counter
  }

    //here we are checking if turnOff is true, if this is the case
    //then the system should "turn off"
    if (turnOff) {
    //here we are setting one so the LED wont turn on when Light button pressed
    x = 1;
    //here we are clearning the display and updating it to "turn it off"
    display.clearDisplay();
    digitalWrite(LED,LOW);
    display.display();
    //here we are returning to make system continue to act as turned off until turned on
    return;  
  } 
  else{
    //here we are setting x to 0 so led can function as normal 
    x = 0;
    //here we are printing to our serial monitor to make sure our system is 
    //working
      Serial.println("SYSTEM ON");

    }

    //here we have an if statement that will check when freezeScreen is true
    if (freezeScreen) {
        //if this case is true then the display should locked
        //with last temperature and lock icon
        //here we clear the display and set the text, color, and cursor
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(5, 9);

        //next depending on if the last measured temperature was in F or C
        //will determine the temperature being printed
        if (gFtemp) {
          //if last temp was F then the last saved F temp will be printed along with the F icon
            display.print(tempF);
            display.drawBitmap(0, 0, images[3], 128, 32, WHITE);
        } else {
          //if last temp was C then the last saved C temp will be printed along with the C icon
            display.print(tempC);
            display.drawBitmap(0, 0, images[2], 128, 32, WHITE);
        }
        
        //here the lock icon will be added onto the screen to let user know the system is locked
        display.drawBitmap(0, 0, images[0], 128, 32, WHITE); 
        //here the screen will update with these settings
        display.display();
        //here we are returning to no longer update the temperature
        return; 
    }

    //here we are printing system unlocked to the serial monitor to know when system is unlocked
    Serial.println("SYSTEM UNLOCKED");
    

  // here we are reading the temperature sensor and then setting it to a float value
  //it is stored to the variable 'temperature'
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  //here we are checking to make sure the sensor is properly reading the temperature
  if (temperature == -127.00) {
    Serial.println("Error: Could not read temperature from sensor.");
    temperature = 0.0;
  } else {
    //here we are printing the temperature to the serial monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  }

  //here we are reading the accelerometer data from MPU6050
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Determine screen orientation based on accelerometer data
  //here we are setting tilt_x to ax to check the rotation in the x direction
  int tilt_x = ax;


  //here we are checking if tilt_x is reading higher than 1000
  //if this is the case then the screen will rotate
  // Only rotate the screen for landscape or upside-down (skip portrait orientation)
  if (tilt_x > 1000) {
    //here board is tilted to the right (landscape)
    if (rotation != 1) {
      rotation = 1;
      display.setRotation(0);  //here we set rotate to landscape
      //here we clear the screen after rotation
      display.fillScreen(SSD1306_BLACK);  
    }
  } else if (tilt_x < -1000) {
    //here if tilt_x is less than -1000 then the screen will rotate upside down
    // to appear as upward in its new position
    if (rotation != 2) {
      rotation = 2;
        //here we are rotating to upside-down
      display.setRotation(2);
      //then clearing the screen after rotation
      display.fillScreen(SSD1306_BLACK); 
    }
  }

  //here we are displaying the temperature on the screen by 
  //clearing the display and setting the text size, color, and cursor
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 9);

  //then depending on if gGtemp is true, the temp will display in farhenheit
    if (gFtemp) {
    temperature = (temperature * 9.0 / 5.0) + 32.0;
    display.print(temperature);
    //along with the F and the unlocked icon
    display.drawBitmap(0, 0, images[3], 128, 32, WHITE);
    display.drawBitmap(0, 0, images[1], 128, 32, WHITE);
    //here we will update the variable tempF with the current temperature incase
    //lock button is pressed 
    tempF = temperature;
    }
    else{
      //if gFtemp is not true, then the temp will display in celcius
      //
    display.print(temperature);
    //along with the C and the unlocked icon
    display.drawBitmap(0, 0, images[2], 128, 32, WHITE);
    display.drawBitmap(0, 0, images[1], 128, 32, WHITE);
    //here we will update the variable tempC with the current temperature incase
    //lock button is pressed 
    tempC = temperature;
    }
    //here the display will be updated with the current temperature
  display.display();
  //here is a small delay to make the screen update at a smoother pace
  delay(500);  
}


ISR(TIMER1_COMPA_vect) {
  //here we are incrementing the inactivity counter every second
  inactivityCounter++;  

  //here we are checking if the inactiving counter has reached 90 or more seconds
  // if this case is true, then the onOffButton ISR will start and make the system
  //turn off. 
  if (inactivityCounter >= TIMER1_COUNTER_MAX) {
    onOffButtonISR();  
    //the inactivity counter will be reset to 0 to be used again when
    //inactivity is detected after system has been turned back on
    inactivityCounter = 0; 
  }
}

//here we are creating an interrupt for when the button has been pressed for 3 seconds
ISR(TIMER2_COMPA_vect)
{
  //here we are incrementing the counter
  gCounterTimer2++;

  //here we are checking if the gCounterTimer2 has been pressed for 3 seconds
  if (gCounterTimer2 >= TIMER2_COUNTER_MAX)
  {
    //if the units button has been pressed for the three seconds then the gFtemp will toggle
    //causing the units to change
    if (digitalRead(UNITS) == HIGH)
    {
    //here we are changing the temperature units
    gFtemp = !gFtemp;
    }

    //here we are resetting the counter back to 0
    gCounterTimer2 = 0;
  }
}

//here we are creating an ISR that will freeze the screen to lock the temp
void lockButtonISR() {
  //here we toggle freezeScreen when Cal button pressed (*lock button didnt work*)
  //this toggle will cause the loop to enter the freezeScreen if statement
  freezeScreen = !freezeScreen;

  //here we are printing to our serial monitor the state of the screen
  //to ensure system is properly working
  if (freezeScreen) {
    Serial.println("Screen Frozen");
    
  } else {
    Serial.println("Screen Unfrozen");
  }

}


//here we are creating an ISR for the onOff button
void onOffButtonISR() {

  //here the turnOff state will toggle when button is pressed
  //to enter turnOff mode
  turnOff = !turnOff;

  //here we are printing to our serial monitor to make sure system is working properly
  if (turnOff) {
    Serial.println("System Off");
    
  } else {
    Serial.println("System On");
  }

}






