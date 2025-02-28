//******************************************************************************
//Serial monitor should be on at all times to make sure user is aware of what system is doing
//Buttons should be pressed for at least 2-3 seconds then released to produce wanted output
//the output has a little delay and will update within 2-3 seconds. Not sure if this is an internal
//issue or if something else is wrong but as long as button is held for 2-3 seconds, outputs
//will be seen after 2-3 seconds
//***************************************************************************

//here we are setting up variables for each pin
#define TEMP_SENSOR_PIN A0  
#define LED_PIN 9           
#define BUTTON_1_PIN 2      // (onoff button on pcb) (onoff for fan)
#define BUTTON_2_PIN 4      // (units button on pcb) (for changing LED intensity)

//here are setting the deault sensitivity level
int sensitivity_level = 2;  
//starting the fan off
bool fan_on = false;  
//storing the calculated average temp value
int avg_temperature = 0;  
//this is for the pwm duty cycle for fan speed
int pwm_duty_cycle = 0;  


void setup() {
    // here we are setting each the pins as an input or output
    pinMode(LED_PIN, OUTPUT);      
    pinMode(BUTTON_1_PIN, INPUT);  
    pinMode(BUTTON_2_PIN, INPUT);  

    //here we are starting serial communication
    Serial.begin(9600);
    Serial.println("Fan Controller Initialized");

    //here we are setting Timer1 for 250 Hz PWM frequency for pin 9 (the led)
    //start by clearing the timers
    TCCR1A = 0; 
    TCCR1B = 0; 

    //here we are setting Timer1 to Fast PWM mode and non-inverting mode
    TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
     //here we are setting prescaler to 64
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); 
    
    //here we are settting a TOP value for 250 Hz PWM frequency
    OCR1A = 249; 
}

void loop() {
    //here we are reading and calculating the average temp coming from the sensor
    avg_temperature = calculate_average_temperature();

    //here we are printing the average temperature to Serial Monitor
    Serial.print("Average Temperature: ");
    Serial.println(avg_temperature);

    
    //here if the fan is on then we adjustive the fan speed 
    //based on the sensitivity and temp
    //otherwise fan is off (led is off)
    if (fan_on) {
        pwm_duty_cycle = calculate_pwm_duty_cycle(avg_temperature, sensitivity_level);
        analogWrite(LED_PIN, pwm_duty_cycle);  
        
        Serial.print("Fan Speed (PWM Duty Cycle): ");
        Serial.println(pwm_duty_cycle);
    } else {
        analogWrite(LED_PIN, 0);  
        Serial.println("Fan is OFF");
    }

    //here we are printing the sensitivity level
    Serial.print("Sensitivity Level: ");
    Serial.println(sensitivity_level);

    //here we are checking if the onoff button has been pressed
    int button_1_state = digitalRead(BUTTON_1_PIN);
    if (button_1_state == HIGH) {  
      //if it was pressed then the fan_on bool will toggle
      //its status will also be printed
        fan_on = !fan_on;  
        Serial.print("Button 1 pressed: Fan status: ");
        Serial.println(fan_on ? "ON" : "OFF");
    }


    ////here we are checking if the units button has been pressed 
    //the units button is pressed to change sensitivity
    int button_2_state = digitalRead(BUTTON_2_PIN);
    if (button_2_state == HIGH) {
      //if the button was pressed then the sensitivity level will update
      //and the results will be printed to the serial monitor
      //it will also always stay within range of 1-3
        sensitivity_level = (sensitivity_level % 3) + 1;  
        Serial.print("Button 2 pressed: New Sensitivity Level: ");
        Serial.println(sensitivity_level);
    }

    delay(50); 
}


//here we are reading the temp from the sensor
int read_temperature() {
  //here we are reading the temp from the sensor
    int sensor_value = analogRead(TEMP_SENSOR_PIN);  
    //then we are converting it to voltages assuming a 5V reference
    float voltage = sensor_value * (5.0 / 1023.0);  
    //here we are converting voltage to temp
    int temperature = voltage * 100;  
    return temperature;
}

//here we are calculate the average temperature over 100 samples (collected over 5 seconds)
int calculate_average_temperature() {
    long sum = 0;
    for (int i = 0; i < 100; i++) {
      //adding up all temps
        sum += read_temperature();  
        delay(50);  
    }
    //returning the average of all temps
    return sum / 100;  
}

//here we are calculating PWM duty cycle based on temperature and sensitivity level
int calculate_pwm_duty_cycle(int temperature, int sensitivity) {
  //here we are mapping temperature to PWM range (0-255)
    int base_pwm = map(temperature, 0, 100, 0, 255);  
    
    //here we are printing our base PWM
    Serial.print("Base PWM: ");
    Serial.println(base_pwm);  

    //here we are adjusting the pwm based on the level of sensitivity
    int adjusted_pwm = base_pwm;
    switch (sensitivity) {
        case 1:  // Low sensitivity (low bright)
            adjusted_pwm = map(base_pwm, 0, 255, 0, 20);
            break;
        case 2:  // Medium sensitivity (med bright)
            adjusted_pwm = map(base_pwm, 0, 255, 0, 110);
            break;
        case 3:  // High sensitivity (high bright)
            adjusted_pwm = map(base_pwm, 0, 255, 0, 255);
            break;
    }

  //here we are contraining the pwm to be within 0 and 255
    return constrain(adjusted_pwm, 0, 255);  // Ensure PWM is within valid range (0-255)
}

